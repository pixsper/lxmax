// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "net.h"

#ifdef WIN_VERSION
#include <comdef.h> 
#include <WinSock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>
#else
#include <ifaddrs.h>
#include <netinet/in.h>
#endif

#include <sstream>
#include <regex>

#include <ext_hashtab.h>

t_class* lx_network_adapter_class;


t_symbol* lx_ip_address_to_sym(const t_lx_ip_address* address)
{
	char buffer[16];
	inet_ntop(AF_INET, address->data, buffer, sizeof(buffer));
	return gensym(buffer);
}

t_max_err lx_ip_address_from_sym(t_symbol* sym, t_lx_ip_address* address)
{
	return inet_pton(AF_INET, sym->s_name, address->data) == 1 ? MAX_ERR_NONE : MAX_ERR_GENERIC;
}

void lx_ip_address_to_directed_broadcast(const t_lx_ip_address* address, const t_lx_ip_address* subnet,
	t_lx_ip_address* broadcast_address)
{
	broadcast_address->data[0] = address->data[0] | ~subnet->data[0];
	broadcast_address->data[1] = address->data[1] | ~subnet->data[1];
	broadcast_address->data[2] = address->data[2] | ~subnet->data[2];
	broadcast_address->data[3] = address->data[3] | ~subnet->data[3];
}



void lx_network_adapter_register_class()
{
	t_class* c;

	c = class_new(LX_NETWORK_ADAPTER_CLASS_NAME, (method)lx_network_adapter_new, (method)lx_network_adapter_free,
		(long)sizeof(t_lx_network_adapter),0L, A_CANT, 0);

	class_register(CLASS_NOBOX, c);
	lx_network_adapter_class = c;
}

t_lx_network_adapter* lx_network_adapter_new(t_symbol* name, t_atom_long index, t_linklist* addresses)
{
 	t_lx_network_adapter* x = (t_lx_network_adapter*)object_alloc(lx_network_adapter_class);
	if (x == NULL)
		return NULL;

	x->name = name;
	x->index = index;
	x->addresses = addresses;

	return x;
}

void lx_network_adapter_free(t_lx_network_adapter* x)
{
	object_free(x->addresses);
}



t_hashtab* lx_network_adapter_enumerate()
{
	t_hashtab* hashtab_adapters = hashtab_new(0);

#ifdef WIN_VERSION

	ULONG addresses_length = DEFAULT_MINIMUM_ENTITIES * sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES adapters = NULL;
	ULONG rt;
	int retry_count = 0;
	const int max_tries = 3;

	do
	{
		adapters = (PIP_ADAPTER_ADDRESSES)sysmem_newptr(addresses_length);

		rt = GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_INCLUDE_PREFIX,
			NULL, adapters, &addresses_length);

		if (rt == ERROR_BUFFER_OVERFLOW)
		{
			sysmem_freeptr(adapters);
			adapters = NULL;
		}
		else
		{
			break;
		}

		++retry_count;

	} while ((rt == ERROR_BUFFER_OVERFLOW) && (retry_count < max_tries));

	if (rt == NO_ERROR)
	{
		PIP_ADAPTER_ADDRESSES current_adapter = adapters;

		while (current_adapter)
		{
			t_linklist* addresses = linklist_new();
			linklist_flags(addresses, OBJ_FLAG_MEMORY);

			PIP_ADAPTER_UNICAST_ADDRESS current_address = current_adapter->FirstUnicastAddress;
			while (current_address)
			{
				if (current_address->Address.lpSockaddr->sa_family != AF_INET)
				{
					current_address = current_address->Next;
					continue;
				}

				t_lx_adapter_address* lx_adapter_address = (t_lx_adapter_address*)sysmem_newptr(sizeof(t_lx_adapter_address));

				struct sockaddr_in* a = (struct sockaddr_in*)current_address->Address.lpSockaddr;
				memcpy(lx_adapter_address->address.data, &a->sin_addr, 4);

				SOCKADDR_IN subnet_mask;
				ConvertLengthToIpv4Mask(current_address->OnLinkPrefixLength, (PULONG)&subnet_mask.sin_addr);
				memcpy(lx_adapter_address->subnet_mask.data, &subnet_mask.sin_addr, 4);

				linklist_append(addresses, lx_adapter_address);
				current_address = current_address->Next;
			}

			t_lx_network_adapter* lx_adapter = lx_network_adapter_new(
				gensym(_bstr_t(current_adapter->FriendlyName)), 
				current_adapter->IfIndex,
				addresses);

			hashtab_store(hashtab_adapters, lx_adapter->name, (t_object*)lx_adapter);
			current_adapter = current_adapter->Next;
		}
	}

	sysmem_freeptr(adapters);

#else

#endif

	return hashtab_adapters;
}

void lx_network_adapter_get_menu_items(t_hashtab* adapters, long* argc, t_atom** argv)
{
	char alloc;

	long key_count = 0;
	t_symbol** keys = NULL;
	hashtab_getkeys(adapters, &key_count, &keys);

	atom_alloc_array(key_count + 2, argc, argv, &alloc);
	atom_setsym(*argv, _lxsym_interface_ip_adapter_global);
	atom_setsym(*argv + 1, _lxsym_interface_ip_adapter_all);

	for (long i = 0; i < key_count; ++i)
	{
		t_symbol* key = *(keys + i);
		t_lx_network_adapter* adapter = NULL;

		if (hashtab_lookup(adapters, key, (t_object**)&adapter) == MAX_ERR_NONE)
			atom_setsym(*argv + i + 2, lx_network_adapter_to_sym(adapter));
	}
}

t_lx_network_adapter* lx_network_adapter_find(t_hashtab* adapters, t_symbol* sym)
{
	const std::regex adapter_sym_regex(R"((.*) \((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})(?:\, \d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})*\))");

	t_lx_network_adapter* result = NULL;

	std::cmatch matches;
	if (std::regex_match(sym->s_name, matches, adapter_sym_regex))
	{
		t_symbol* name = gensym(matches[1].str().c_str());
		t_lx_ip_address ip;
		if (inet_pton(AF_INET, matches[2].str().c_str(), ip.data) != 1)
			return NULL;
		
		long key_count = 0;
		t_symbol** keys = NULL;
		hashtab_getkeys(adapters, &key_count, &keys);

		for (long i = 0; i < key_count; ++i)
		{
			t_symbol* key = *(keys + i);
			t_lx_network_adapter* adapter = NULL;

			if (hashtab_lookup(adapters, key, (t_object**)&adapter) == MAX_ERR_NONE)
			{
				if (adapter->name == name)
				{
					result = adapter;
					hashtab_keyflags(adapters, key, OBJ_FLAG_REF);
					break;
				}
			}
		}

		if (result == NULL)
		{
			for (long i = 0; i < key_count; ++i)
			{
				t_symbol* key = *(keys + i);
				t_lx_network_adapter* adapter = NULL;

				if (hashtab_lookup(adapters, key, (t_object**)&adapter) == MAX_ERR_NONE)
				{
					t_lx_adapter_address* adapter_address = (t_lx_adapter_address*)linklist_getindex(adapter->addresses, 0);

					while (adapter_address)
					{
						if (memcmp(adapter_address->address.data, ip.data, 4) == 0)
						{
							result = adapter;
							hashtab_keyflags(adapters, key, OBJ_FLAG_REF);
							break;
						}

						linklist_next(adapter->addresses, adapter_address, (void**)&adapter_address);
					}
				}
			}
		}
	}

	return result;
}





t_symbol* lx_network_adapter_to_sym(const t_lx_network_adapter* adapter)
{
	std::stringstream menu_item;
	menu_item << adapter->name->s_name;

	bool is_first = true;

	t_lx_adapter_address* adapter_address = (t_lx_adapter_address*)linklist_getindex(adapter->addresses, 0);

	while (adapter_address)
	{
		if (is_first)
		{
			menu_item << " (";
			is_first = false;
		}
		else
		{
			menu_item << ", ";
		}

		menu_item << lx_ip_address_to_sym(&adapter_address->address)->s_name;

		linklist_next(adapter->addresses, adapter_address, (void**)&adapter_address);
	}

	if (!is_first)
		menu_item << ")";

	return gensym(menu_item.str().c_str());
}

t_linklist* lx_network_adapter_get_broadcast_ips(const t_lx_network_adapter* adapter)
{
	t_linklist* broadcast_ips = linklist_new();
	linklist_flags(broadcast_ips, OBJ_FLAG_DATA);

	t_lx_adapter_address* adapter_address = (t_lx_adapter_address*)linklist_getindex(adapter->addresses, 0);

	while (adapter_address)
	{
		t_lx_ip_address* address = (t_lx_ip_address*)sysmem_newptr(sizeof(t_lx_ip_address));
		lx_ip_address_to_directed_broadcast(&adapter_address->address, &adapter_address->subnet_mask, address);
		linklist_append(broadcast_ips, address);

		linklist_next(adapter->addresses, adapter_address, (void**)&adapter_address);
	}

	return broadcast_ips;
}