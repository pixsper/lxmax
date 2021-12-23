// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "common.h"

#include "net.h"

BEGIN_USING_C_LINKAGE

typedef struct _lx_service
{
	t_object object;

	t_linklist* universes;

	t_hashtab* network_adapters;
	t_lx_network_adapter* global_artnet_adapter;
	t_lx_network_adapter* global_sacn_adapter;

	t_symbol* global_artnet_interface;
	t_lx_ip_address global_artnet_unicast_addresses[LX_UNICAST_ADDRESSES_MAX];
	long global_artnet_unicast_addresses_count;
	t_bool artnet_sync_packets_enable;

	t_symbol* global_sacn_interface;
	t_lx_ip_address global_sacn_unicast_addresses[LX_UNICAST_ADDRESSES_MAX];
	long global_sacn_unicast_addresses_count;
	t_bool sacn_sync_packets_enable;
	long sacn_sync_address;

	t_atom_float framerate;
	t_bool allow_nondmx_framerate;
	t_bool force_framerate;
	t_bool output_empty_universes;

} t_lx_service;

typedef struct _lx_universe_config t_lx_universe_config;

DLL_API void lxmax_service_universes_add(t_lx_service* x, t_lx_universe_config* universe_config, long index);
DLL_API void lxmax_service_universes_remove(t_lx_service* x, t_lx_universe_config* universe_config);
DLL_API void lxmax_service_universes_clear(t_lx_service* x);

END_USING_C_LINKAGE