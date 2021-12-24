// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_net.h"

#ifdef WIN_VERSION

#include <WinSock2.h>
#include <iphlpapi.h>

t_hashtab* lx_network_adapter_enumerate()
{
	t_hashtab* hashtab_adapters = hashtab_new(0);

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
				memcpy(lx_adapter_address->address.data, &a->sin_addr, LX_IPV4_ADDRESS_LENGTH);

				SOCKADDR_IN subnet_mask;
				ConvertLengthToIpv4Mask(current_address->OnLinkPrefixLength, (PULONG)&subnet_mask.sin_addr);
				memcpy(lx_adapter_address->subnet_mask.data, &subnet_mask.sin_addr, LX_IPV4_ADDRESS_LENGTH);

				linklist_append(addresses, lx_adapter_address);
				current_address = current_address->Next;
			}

			char friendly_name[256];
			wcstombs(friendly_name, current_adapter->FriendlyName, sizeof(friendly_name));

			t_lx_network_adapter* lx_adapter = lx_network_adapter_new(gensym(friendly_name), 
				current_adapter->IfIndex,
				addresses);

			hashtab_store(hashtab_adapters, lx_adapter->name, (t_object*)lx_adapter);
			current_adapter = current_adapter->Next;
		}
	}

	sysmem_freeptr(adapters);

	return hashtab_adapters;
}

#endif