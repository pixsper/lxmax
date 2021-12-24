// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "ext_prefix.h"
#include "ext_mess.h"

BEGIN_USING_C_LINKAGE

#define LX_CLASSNAME_SERVICE "LXMax"
#define LX_CLASSNAME_UNIVERSE_CONFIG "lx.universeconfig"
#define LX_CLASSNAME_NETWORK_ADAPTER "lx.networkadapter"

#define LXSYMNAME____LXMAX_SERVICE "___lxmax_service"

#define LXSYMNAME_UNIVERSE_CONFIG_LABEL "label"
#define LXSYMNAME_UNIVERSE_CONFIG_TYPE "type"
#define LXSYMNAME_UNIVERSE_CONFIG_ENABLE "enable"
#define LXSYMNAME_UNIVERSE_CONFIG_STATUS "status"
#define LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL "protocol"
#define LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_MODE "protocol_mode"
#define LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_INTERFACE "protocol_interface"
#define LXSYMNAME_UNIVERSE_CONFIG_INTERNAL_UNIVERSE "internal_universe"
#define LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_UNIVERSE "protocol_universe"
#define LXSYMNAME_UNIVERSE_CONFIG_PRIORITY "priority"
#define LXSYMNAME_UNIVERSE_CONFIG_UNICAST_ADDRESSES "unicast_addresses"

#define LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE "global_artnet_interface"
#define LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES "global_artnet_unicast_addresses"
#define LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE "artnet_sync_packets_enable"
#define LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE "global_sacn_interface"
#define LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES "global_sacn_unicast_addresses"
#define LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE "sacn_sync_packets_enable"
#define LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS "artnet_sync_address"
#define LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE "max_framerate"
#define LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE "allow_non_dmx_framerate"
#define LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE "force_framerate"
#define LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES "output_empty_universes"

typedef struct _lx_common_symbols_table
{
    t_symbol* s_lxmaxclass_service;
    t_symbol* s_lxmaxclass_universeconfig;
    t_symbol* s_lxmaxclass_networkadapter;

    t_symbol* s____lxmax_service;

    t_symbol* s_lxmax;

    t_symbol* s_address;
    t_symbol* s_addresses;
    t_symbol* s_subnet_mask;
    t_symbol* s_universes;
    t_symbol* s_not_applicable;
    t_symbol* s_unknown;

	t_symbol* s_lxmax_version_major;
	t_symbol* s_lxmax_version_minor;
	t_symbol* s_lxmax_version_bugfix;

    t_symbol* s_universe_config_label;
    t_symbol* s_universe_config_type;
    t_symbol* s_universe_config_enable;
    t_symbol* s_universe_config_status;
    t_symbol* s_universe_config_protocol;
    t_symbol* s_universe_config_protocol_mode;
    t_symbol* s_universe_config_protocol_interface;
    t_symbol* s_universe_config_internal_universe;
    t_symbol* s_universe_config_protocol_universe;
    t_symbol* s_universe_config_priority;
    t_symbol* s_universe_config_unicast_addresses;

    t_symbol* s_global_config_global_artnet_interface;
    t_symbol* s_global_config_global_artnet_unicast_addresses;
    t_symbol* s_global_config_artnet_sync_packets_enable;
    t_symbol* s_global_config_global_sacn_interface;
    t_symbol* s_global_config_global_sacn_unicast_addresses;
    t_symbol* s_global_config_sacn_sync_packets_enable;
    t_symbol* s_global_config_sacn_sync_address;
    t_symbol* s_global_config_max_framerate;
    t_symbol* s_global_config_allow_non_dmx_framerate;
    t_symbol* s_global_config_force_framerate;
    t_symbol* s_global_config_output_empty_universes;

    t_symbol* s_type_input;
    t_symbol* s_type_output;

    t_symbol* s_protocol_artnet;
    t_symbol* s_protocol_sacn;
    t_symbol* s_protocol_enttec_dmxusbpro;
    t_symbol* s_protocol_enttec_opendmxusb;

    t_symbol* s_type_input_protocol_artnet_mode_input;
    t_symbol* s_type_output_protocol_artnet_mode_broadcast;
    t_symbol* s_type_output_protocol_artnet_mode_unicast;
    t_symbol* s_type_all_protocol_sacn_mode_multicast;
    t_symbol* s_type_all_protocol_sacn_mode_unicast;
    t_symbol* s_type_input_protocol_dmxusbpro_mode_input;
    t_symbol* s_type_output_protocol_dmxusbpro_mode_output;
    t_symbol* s_type_output_protocol_opendmxusb_mode_output;

    t_symbol* s_interface_none;
    t_symbol* s_interface_ip_adapter_global;
    t_symbol* s_interface_ip_adapter_all;

    t_symbol* s_ip_address_all;
    t_symbol* s_ip_address_broadcast;

    t_symbol* s_service_notify_universes_updated;

	t_symbol* s_universe_types;
	t_symbol* s_universe_protocols;
	t_symbol* s_universe_protocol_modes;
	t_symbol* s_universe_protocol_interfaces;

} t_lx_common_symbols_table;

#define _lxclassnamesym_service (_lx_common_symbols->s_lxmaxclass_service)
#define _lxclassnamesym_universeconfig (_lx_common_symbols->s_lxmaxclass_universeconfig)
#define _lxclassnamesym_networkadapter (_lx_common_symbols->s_lxmaxclass_networkadapter)

#define _lxsym____lxmax_service (_lx_common_symbols->s____lxmax_service)

#define _lxsym_lxmax (_lx_common_symbols->s_lxmax)

#define _lxsym_address (_lx_common_symbols->s_address)
#define _lxsym_addresses (_lx_common_symbols->s_addresses)
#define _lxsym_subnet_mask (_lx_common_symbols->s_subnet_mask)
#define _lxsym_universes (_lx_common_symbols->s_universes)
#define _lxsym_not_applicable (_lx_common_symbols->s_not_applicable)
#define _lxsym_unknown (_lx_common_symbols->s_unknown)

#define _lxsym_lxmax_version_major (_lx_common_symbols->s_lxmax_version_major)
#define _lxsym_lxmax_version_minor (_lx_common_symbols->s_lxmax_version_minor)
#define _lxsym_lxmax_version_bugfix (_lx_common_symbols->s_lxmax_version_bugfix)

#define _lxsym_universe_config_label (_lx_common_symbols->s_universe_config_label)
#define _lxsym_universe_config_type (_lx_common_symbols->s_universe_config_type)
#define _lxsym_universe_config_enable (_lx_common_symbols->s_universe_config_enable)
#define _lxsym_universe_config_status (_lx_common_symbols->s_universe_config_status)
#define _lxsym_universe_config_protocol (_lx_common_symbols->s_universe_config_protocol)
#define _lxsym_universe_config_protocol_mode (_lx_common_symbols->s_universe_config_protocol_mode)
#define _lxsym_universe_config_protocol_interface (_lx_common_symbols->s_universe_config_protocol_interface)
#define _lxsym_universe_config_internal_universe (_lx_common_symbols->s_universe_config_internal_universe)
#define _lxsym_universe_config_protocol_universe (_lx_common_symbols->s_universe_config_protocol_universe)
#define _lxsym_universe_config_priority (_lx_common_symbols->s_universe_config_priority)
#define _lxsym_universe_config_unicast_addresses (_lx_common_symbols->s_universe_config_unicast_addresses)

#define _lxsym_global_config_global_artnet_interface (_lx_common_symbols->s_global_config_global_artnet_interface)
#define _lxsym_global_config_global_artnet_unicast_addresses (_lx_common_symbols->s_global_config_global_artnet_unicast_addresses)
#define _lxsym_global_config_artnet_sync_packets_enable (_lx_common_symbols->s_global_config_artnet_sync_packets_enable)
#define _lxsym_global_config_global_sacn_interface (_lx_common_symbols->s_global_config_global_sacn_interface)
#define _lxsym_global_config_global_sacn_unicast_addresses (_lx_common_symbols->s_global_config_global_sacn_unicast_addresses)
#define _lxsym_global_config_sacn_sync_packets_enable (_lx_common_symbols->s_global_config_sacn_sync_packets_enable)
#define _lxsym_global_config_sacn_sync_address (_lx_common_symbols->s_global_config_sacn_sync_address)
#define _lxsym_global_config_max_framerate (_lx_common_symbols->s_global_config_max_framerate)
#define _lxsym_global_config_allow_non_dmx_framerate (_lx_common_symbols->s_global_config_allow_non_dmx_framerate)
#define _lxsym_global_config_force_framerate (_lx_common_symbols->s_global_config_force_framerate)
#define _lxsym_global_config_output_empty_universes (_lx_common_symbols->s_global_config_output_empty_universes)

#define _lxsym_type_input (_lx_common_symbols->s_type_input)
#define _lxsym_type_output (_lx_common_symbols->s_type_output)

#define _lxsym_protocol_artnet (_lx_common_symbols->s_protocol_artnet)
#define _lxsym_protocol_sacn (_lx_common_symbols->s_protocol_sacn)
#define _lxsym_protocol_enttec_dmxusbpro (_lx_common_symbols->s_protocol_enttec_dmxusbpro)
#define _lxsym_protocol_enttec_opendmxusb (_lx_common_symbols->s_protocol_enttec_opendmxusb)

#define _lxsym_type_input_protocol_artnet_mode_input (_lx_common_symbols->s_type_input_protocol_artnet_mode_input)
#define _lxsym_type_output_protocol_artnet_mode_broadcast (_lx_common_symbols->s_type_output_protocol_artnet_mode_broadcast)
#define _lxsym_type_output_protocol_artnet_mode_unicast (_lx_common_symbols->s_type_output_protocol_artnet_mode_unicast)
#define _lxsym_type_all_protocol_sacn_mode_multicast (_lx_common_symbols->s_type_all_protocol_sacn_mode_multicast)
#define _lxsym_type_all_protocol_sacn_mode_unicast (_lx_common_symbols->s_type_all_protocol_sacn_mode_unicast)
#define _lxsym_type_input_protocol_dmxusbpro_mode_input (_lx_common_symbols->s_type_input_protocol_dmxusbpro_mode_input)
#define _lxsym_type_output_protocol_dmxusbpro_mode_output (_lx_common_symbols->s_type_output_protocol_dmxusbpro_mode_output)
#define _lxsym_type_output_protocol_opendmxusb_mode_output (_lx_common_symbols->s_type_output_protocol_opendmxusb_mode_output)

#define _lxsym_interface_none (_lx_common_symbols->s_interface_none)
#define _lxsym_interface_ip_adapter_global (_lx_common_symbols->s_interface_ip_adapter_global)
#define _lxsym_interface_ip_adapter_all (_lx_common_symbols->s_interface_ip_adapter_all)


#define _lxsym_ip_address_all (_lx_common_symbols->s_ip_address_all)
#define _lxsym_ip_address_broadcast (_lx_common_symbols->s_ip_address_broadcast)

#define _lxsym_service_notify_universes_updated (_lx_common_symbols->s_service_notify_universes_updated)

#define _lxsym_universe_types (_lx_common_symbols->s_universe_types)
#define _lxsym_universe_protocols (_lx_common_symbols->s_universe_protocols)
#define _lxsym_universe_protocol_modes (_lx_common_symbols->s_universe_protocol_modes)
#define _lxsym_universe_protocol_interfaces (_lx_common_symbols->s_universe_protocol_interfaces)

extern t_lx_common_symbols_table *_lx_common_symbols;

void lx_common_symbols_init();

END_USING_C_LINKAGE
