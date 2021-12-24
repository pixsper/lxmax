// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "lx_common.h"

#include "lx_net.h"
#include "lx_universe_config.h"

BEGIN_USING_C_LINKAGE

#ifdef __cplusplus
namespace lxmax { class dmx_io_service; }
typedef lxmax::dmx_io_service t_dmx_io_service;
#else
typedef void t_dmx_io_service;
#endif

typedef struct _lx_service
{
	t_object object;
	t_object* registered_object;

	t_qelem* qelem_save;

	t_hashtab* universes;
	t_bool is_initial_load_complete;

	t_hashtab* network_adapters;
	t_hashtab* serial_ports;

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

	t_atom_float max_framerate;
	t_bool allow_nondmx_framerate;
	t_bool force_framerate;
	t_bool output_empty_universes;

	t_clock* io_clock;

	t_systhread io_thread;
	t_dmx_io_service* dmx_io_service;

} t_lx_service;

DLL_API t_lx_service* lx_service_subscribe(t_object* x);
DLL_API t_max_err lx_service_unsubscribe(t_object* x, t_lx_service* service);

DLL_API void lx_service_universes_add(t_lx_service* x, t_lx_universe_config* universe_config);
DLL_API void lx_service_universes_remove(t_lx_service* x, t_lx_universe_config* universe_config);
DLL_API void lx_service_universes_clear(t_lx_service* x);
DLL_API void lx_service_universes_quick_patch(t_lx_service* x, long patch_count,
                                              e_lx_universe_type type, e_lx_universe_protocol protocol,
                                              t_symbol* protocol_mode, t_symbol* protocol_interface,
                                              long start_internal_universe, long start_protocol_universe);

END_USING_C_LINKAGE