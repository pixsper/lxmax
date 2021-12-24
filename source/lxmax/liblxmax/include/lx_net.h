// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "lx_common.h"

BEGIN_USING_C_LINKAGE

#define LX_IPV4_ADDRESS_LENGTH 4
#define LX_UNICAST_ADDRESSES_MAX 32

typedef struct _lx_ip_address
{
	uint8_t data[4];

} t_lx_ip_address;

typedef struct _lx_adapter_address
{
	t_lx_ip_address address;
	t_lx_ip_address subnet_mask;

} t_lx_adapter_address;

typedef struct _lx_network_adapter
{
	t_object object;

	t_symbol* name;
	t_atom_long index;
	t_linklist* addresses;

} t_lx_network_adapter;

extern t_class* lx_network_adapter_class;

DLL_API t_symbol* lx_ip_address_to_sym(const t_lx_ip_address* address);
DLL_API t_max_err lx_ip_address_from_sym(t_symbol* sym, t_lx_ip_address* address);
DLL_API void lx_ip_address_to_directed_broadcast(const t_lx_ip_address* address, const t_lx_ip_address* subnet, t_lx_ip_address* broadcast_address);


void lx_network_adapter_register_class();
DLL_API t_lx_network_adapter* lx_network_adapter_new(t_symbol* name, t_atom_long index, t_linklist* addresses);
DLL_API void lx_network_adapter_free(t_lx_network_adapter* x);

DLL_API t_hashtab* lx_network_adapter_enumerate();
DLL_API void lx_network_adapter_get_menu_items(t_hashtab* adapters, long* argc, t_atom** argv);
DLL_API t_max_err lx_network_adapter_find(t_hashtab* adapters, t_symbol* sym, t_lx_network_adapter** adapter);

DLL_API t_symbol* lx_network_adapter_to_sym(const t_lx_network_adapter* adapter);
DLL_API t_linklist* lx_network_adapter_get_broadcast_ips(const t_lx_network_adapter* adapter);




END_USING_C_LINKAGE