// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_net.h"

#ifdef MAC_VERSION

#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

t_hashtab* lx_network_adapter_enumerate()
{
	t_hashtab* hashtab_adapters = hashtab_new(0);

	struct ifaddrs* addresses;
	if (getifaddrs(&addresses) == 0)
	{
		struct ifaddrs* current_address;
		for (current_address = addresses; current_address != NULL; current_address = current_address->ifa_next)
		{
			if (current_address->ifa_addr == NULL ||
			    current_address->ifa_addr->sa_family != AF_INET)
			{
				continue;
			}

			t_symbol* name = gensym(current_address->ifa_name);

			t_lx_network_adapter* adapter = NULL;
			if (hashtab_lookup(hashtab_adapters, name, (t_object**)&adapter) != MAX_ERR_NONE)
			{
				t_linklist* linklist_addresses = linklist_new();
				linklist_flags(linklist_addresses, OBJ_FLAG_MEMORY);
				adapter = lx_network_adapter_new(name, if_nametoindex(current_address->ifa_name), linklist_addresses);
				hashtab_store(hashtab_adapters, name, (t_object*)adapter);
			}

			t_lx_adapter_address* adapter_address = (t_lx_adapter_address*)sysmem_newptr(sizeof(t_lx_adapter_address));

			in_addr_t addr = ((struct sockaddr_in*)current_address->ifa_addr)->sin_addr.s_addr;
			memcpy(adapter_address->address.data, &addr, LX_IPV4_ADDRESS_LENGTH);
			in_addr_t subnet_addr = ((struct sockaddr_in*)current_address->ifa_netmask)->sin_addr.s_addr;
			memcpy(adapter_address->subnet_mask.data, &subnet_addr, LX_IPV4_ADDRESS_LENGTH);

			linklist_append(adapter->addresses, adapter_address);
		}

		freeifaddrs(addresses);
	}

	return hashtab_adapters;
}

#endif