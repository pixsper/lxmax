// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_net.h"

#if defined WIN_VERSION
#include <WS2tcpip.h>
#elif defined MAC_VERSION
#include <arpa/inet.h>
#endif

#include "lx_helpers.h"

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

	c = class_new(LX_CLASSNAME_NETWORK_ADAPTER, (method)lx_network_adapter_new, (method)lx_network_adapter_free,
	              (long)sizeof(t_lx_network_adapter), 0L, A_CANT, 0);

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



void lx_network_adapter_get_menu_items(t_hashtab* adapters, long* argc, t_atom** argv)
{
	char alloc;

	long key_count = 0;
	t_symbol** keys = NULL;
	if (hashtab_getkeys(adapters, &key_count, &keys) == MAX_ERR_NONE)
	{
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

		if (keys)
			sysmem_freeptr(keys);
	}
}

t_max_err lx_network_adapter_find(t_hashtab* adapters, t_symbol* sym, t_lx_network_adapter** adapter)
{
	t_max_err result = MAX_ERR_NONE;

	const char* adapter_sym_regex = "(.*) \\((\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})(?:\\, \\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})*\\)";

	long matches_count = 0;
	t_symbol** matches = NULL;
	if (lx_helpers_regex_matches(sym, adapter_sym_regex, &matches_count, &matches) == MAX_ERR_NONE)
	{
		t_symbol* name = matches[1];
		t_lx_ip_address ip;
		if (inet_pton(AF_INET, matches[2]->s_name, ip.data) == 1)
		{
			long key_count = 0;
			t_symbol** keys = NULL;
			if (hashtab_getkeys(adapters, &key_count, &keys) == MAX_ERR_NONE)
			{
				for (long i = 0; i < key_count; ++i)
				{
					t_symbol* key = *(keys + i);
					t_lx_network_adapter* a = NULL;

					if (hashtab_lookup(adapters, key, (t_object**)&a) == MAX_ERR_NONE)
					{
						if (a->name == name)
						{
							*adapter = a;
							break;
						}

					}
				}

				if (!result)
				{
					for (long i = 0; i < key_count; ++i)
					{
						t_symbol* key = *(keys + i);
						t_lx_network_adapter* a = NULL;

						if (hashtab_lookup(adapters, key, (t_object**)&a) == MAX_ERR_NONE)
						{
							t_lx_adapter_address* adapter_address = (t_lx_adapter_address*)linklist_getindex(
									a->addresses, 0);

							while (adapter_address)
							{
								if (memcmp(adapter_address->address.data, ip.data, 4) == 0)
								{
									*adapter = a;
									break;
								}

								linklist_next(a->addresses, adapter_address, (void**)&adapter_address);
							}
						}
					}
				}

				if (keys)
					sysmem_freeptr(keys);
			}
		}

		if (matches)
			sysmem_freeptr(matches);
	}
	else
	{
		result = MAX_ERR_GENERIC;
	}

	return result;
}





t_symbol* lx_network_adapter_to_sym(const t_lx_network_adapter* adapter)
{
	const long addresses_count = linklist_getsize(adapter->addresses);
	t_symbol** addresses = (t_symbol**)sysmem_newptr(sizeof(t_symbol*) * addresses_count);

	long i = 0;
	t_lx_adapter_address* adapter_address = (t_lx_adapter_address*)linklist_getindex(adapter->addresses, 0);
	while (adapter_address && i < addresses_count)
	{
		addresses[i++] = lx_ip_address_to_sym(&adapter_address->address);
		linklist_next(adapter->addresses, adapter_address, (void**)&adapter_address);
	}

	t_symbol* addresses_list = lx_helpers_symbol_join(addresses_count, addresses, ", ");

	if (addresses)
		sysmem_freeptr(addresses);
	
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s (%s)", adapter->name->s_name, addresses_list->s_name);

	return gensym(buffer);
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