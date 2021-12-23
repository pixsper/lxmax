// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <ext.h>
#include <ext_obex.h>
#include <commonsyms.h>

#include "version.h"

#ifdef liblxmax_EXPORTS
#ifdef _MSC_VER
#define DLL_API __declspec(dllexport)
#elif defined __GNUC__
#define DLL_API __attribute__ ((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define DLL_API __declspec(dllimport)
#elif defined __GNUC__
#define DLL_API __attribute__ ((visibility ("hidden")))
#endif
#endif

BEGIN_USING_C_LINKAGE

typedef struct _uuid
{
	uint8_t data[16];

} t_lx_uuid;

#define LX_SERVICE_CLASS_NAME "lx.service"
#define LX_UNIVERSE_CONFIG_CLASS_NAME "lx.universeconfig"
#define LX_NETWORK_ADAPTER_CLASS_NAME "lx.networkadapter"
#define LX_SERVICE_SYMBOL_NAME "___lxmax_service"
#define LX_PREFERENCES_FILE_NAME "lxmaxpreferences.json"

DLL_API extern t_symbol* _lxsym_lxmax_service;
DLL_API extern t_symbol* _lxsym_lxmax;

DLL_API extern t_symbol* _lxsym_address;
DLL_API extern t_symbol* _lxsym_addresses;
DLL_API extern t_symbol* _lxsym_subnet_mask;
DLL_API extern t_symbol* _lxsym_universes;
DLL_API extern t_symbol* _lxsym_not_applicable;

#define LX_UNIVERSE_CONFIG_LABEL_SYMBOL_NAME "label"
#define LX_UNIVERSE_CONFIG_TYPE_SYMBOL_NAME "type"
#define LX_UNIVERSE_CONFIG_ENABLE_SYMBOL_NAME "enable"
#define LX_UNIVERSE_CONFIG_PROTOCOL_SYMBOL_NAME "protocol"
#define LX_UNIVERSE_CONFIG_PROTOCOL_MODE_SYMBOL_NAME "protocol_mode"
#define LX_UNIVERSE_CONFIG_PROTOCOL_INTERFACE_SYMBOL_NAME "protocol_interface"
#define LX_UNIVERSE_CONFIG_INTERNAL_UNIVERSE_SYMBOL_NAME "internal_universe"
#define LX_UNIVERSE_CONFIG_PROTOCOL_UNIVERSE_SYMBOL_NAME "protocol_universe"
#define LX_UNIVERSE_CONFIG_PRIORITY_SYMBOL_NAME "priority"
#define LX_UNIVERSE_CONFIG_UNICAST_ADDRESSES_SYMBOL_NAME "unicast_addresses"

#define LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE "global_artnet_interface"
#define LX_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES "global_artnet_unicast_addresses"
#define LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE "artnet_sync_packets_enable"
#define LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE "global_sacn_interface"
#define LX_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES "global_sacn_unicast_addresses"
#define LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE "sacn_sync_packets_enable"
#define LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS "artnet_sync_address"
#define LX_GLOBAL_CONFIG_FRAMERATE "framerate"
#define LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE "allow_non_dmx_framerate"
#define LX_GLOBAL_CONFIG_FORCE_FRAMERATE "force_framerate"
#define LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES "output_empty_universes"


DLL_API extern t_symbol* _lxsym_universe_config_label;
DLL_API extern t_symbol* _lxsym_universe_config_type;
DLL_API extern t_symbol* _lxsym_universe_config_enable;
DLL_API extern t_symbol* _lxsym_universe_config_protocol;
DLL_API extern t_symbol* _lxsym_universe_config_protocol_mode;
DLL_API extern t_symbol* _lxsym_universe_config_protocol_interface;
DLL_API extern t_symbol* _lxsym_universe_config_internal_universe;
DLL_API extern t_symbol* _lxsym_universe_config_protocol_universe;
DLL_API extern t_symbol* _lxsym_universe_config_priority;
DLL_API extern t_symbol* _lxsym_universe_config_unicast_addresses;

DLL_API extern t_symbol* _lxsym_global_config_global_artnet_interface;
DLL_API extern t_symbol* _lxsym_global_config_global_artnet_unicast_addresses;
DLL_API extern t_symbol* _lxsym_global_config_artnet_sync_packets_enable;
DLL_API extern t_symbol* _lxsym_global_config_global_sacn_interface;
DLL_API extern t_symbol* _lxsym_global_config_global_sacn_unicast_addresses;
DLL_API extern t_symbol* _lxsym_global_config_sacn_sync_packets_enable;
DLL_API extern t_symbol* _lxsym_global_config_sacn_sync_address;
DLL_API extern t_symbol* _lxsym_global_config_framerate;
DLL_API extern t_symbol* _lxsym_global_config_allow_non_dmx_framerate;
DLL_API extern t_symbol* _lxsym_global_config_force_framerate;
DLL_API extern t_symbol* _lxsym_global_config_output_empty_universes;

DLL_API extern t_symbol* _lxsym_type_input;
DLL_API extern t_symbol* _lxsym_type_output;

DLL_API extern t_symbol* _lxsym_protocol_artnet;
DLL_API extern t_symbol* _lxsym_protocol_sacn;

DLL_API extern t_symbol* _lxsym_type_input_protocol_artnet_mode_input;
DLL_API extern t_symbol* _lxsym_type_output_protocol_artnet_mode_broadcast;
DLL_API extern t_symbol* _lxsym_type_output_protocol_artnet_mode_unicast;
DLL_API extern t_symbol* _lxsym_type_all_protocol_sacn_mode_multicast;
DLL_API extern t_symbol* _lxsym_type_all_protocol_sacn_mode_unicast;

DLL_API extern t_symbol* _lxsym_interface_ip_adapter_global;
DLL_API extern t_symbol* _lxsym_interface_ip_adapter_all;

DLL_API extern t_symbol* _lxsym_ip_address_all;
DLL_API extern t_symbol* _lxsym_ip_address_broadcast;

DLL_API extern t_symbol* _lxsym_service_notify_universes_updated;



DLL_API void lx_sym_init();

END_USING_C_LINKAGE