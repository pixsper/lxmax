// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_common.h"

#include "lx_net.h"
#include "lx_universe_config.h"

static t_lx_common_symbols_table s_lx_common_syms;

t_lx_common_symbols_table* lx_common_symbols_gettable()
{
    s_lx_common_syms.s_lxmaxclass_service = gensym(LX_CLASSNAME_SERVICE);
    s_lx_common_syms.s_lxmaxclass_universeconfig = gensym(LX_CLASSNAME_UNIVERSE_CONFIG);
    s_lx_common_syms.s_lxmaxclass_networkadapter = gensym(LX_CLASSNAME_NETWORK_ADAPTER);

	s_lx_common_syms.s____lxmax_service = gensym(LXSYMNAME____LXMAX_SERVICE);

    s_lx_common_syms.s_lxmax = gensym("lxmax");

    s_lx_common_syms.s_address = gensym("address");
    s_lx_common_syms.s_addresses = gensym("addresses");
    s_lx_common_syms.s_subnet_mask = gensym("subnet_mask");
    s_lx_common_syms.s_universes = gensym("universes");
    s_lx_common_syms.s_not_applicable = gensym("n/a");
    s_lx_common_syms.s_unknown = gensym("[unknown]");

	s_lx_common_syms.s_lxmax_version_major = gensym("lxmax_version_major");
	s_lx_common_syms.s_lxmax_version_minor = gensym("lxmax_version_minor");
	s_lx_common_syms.s_lxmax_version_bugfix = gensym("lxmax_version_bugfix");

    s_lx_common_syms.s_universe_config_label = gensym(LXSYMNAME_UNIVERSE_CONFIG_LABEL);
    s_lx_common_syms.s_universe_config_type = gensym(LXSYMNAME_UNIVERSE_CONFIG_TYPE);
    s_lx_common_syms.s_universe_config_enable = gensym(LXSYMNAME_UNIVERSE_CONFIG_ENABLE);
    s_lx_common_syms.s_universe_config_status = gensym(LXSYMNAME_UNIVERSE_CONFIG_STATUS);
    s_lx_common_syms.s_universe_config_protocol = gensym(LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL);
    s_lx_common_syms.s_universe_config_protocol_mode = gensym(LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_MODE);
    s_lx_common_syms.s_universe_config_protocol_interface = gensym(LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_INTERFACE);
    s_lx_common_syms.s_universe_config_internal_universe = gensym(LXSYMNAME_UNIVERSE_CONFIG_INTERNAL_UNIVERSE);
    s_lx_common_syms.s_universe_config_protocol_universe = gensym(LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_UNIVERSE);
    s_lx_common_syms.s_universe_config_priority = gensym(LXSYMNAME_UNIVERSE_CONFIG_PRIORITY);
    s_lx_common_syms.s_universe_config_unicast_addresses = gensym(LXSYMNAME_UNIVERSE_CONFIG_UNICAST_ADDRESSES);

    s_lx_common_syms.s_global_config_global_artnet_interface = gensym(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE);
    s_lx_common_syms.s_global_config_global_artnet_unicast_addresses = gensym(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES);
    s_lx_common_syms.s_global_config_artnet_sync_packets_enable = gensym(LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE);
    s_lx_common_syms.s_global_config_global_sacn_interface = gensym(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE);
    s_lx_common_syms.s_global_config_global_sacn_unicast_addresses = gensym(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES);
    s_lx_common_syms.s_global_config_sacn_sync_packets_enable = gensym(LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE);
    s_lx_common_syms.s_global_config_sacn_sync_address = gensym(LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS);
    s_lx_common_syms.s_global_config_max_framerate = gensym(LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE);
    s_lx_common_syms.s_global_config_allow_non_dmx_framerate = gensym(LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE);
    s_lx_common_syms.s_global_config_force_framerate = gensym(LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE);
    s_lx_common_syms.s_global_config_output_empty_universes = gensym(LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES);

    s_lx_common_syms.s_type_input = gensym("input");
    s_lx_common_syms.s_type_output = gensym("output");

    s_lx_common_syms.s_protocol_artnet = gensym("Art-Net");
    s_lx_common_syms.s_protocol_sacn = gensym("sACN");
    s_lx_common_syms.s_protocol_enttec_dmxusbpro = gensym("Enttec DMX USB Pro");
    s_lx_common_syms.s_protocol_enttec_opendmxusb = gensym("Enttec Open DMX USB");

    s_lx_common_syms.s_type_input_protocol_artnet_mode_input= gensym("input");
    s_lx_common_syms.s_type_output_protocol_artnet_mode_broadcast = gensym("broadcast");
    s_lx_common_syms.s_type_output_protocol_artnet_mode_unicast = gensym("unicast");
    s_lx_common_syms.s_type_all_protocol_sacn_mode_multicast = gensym("multicast");
    s_lx_common_syms.s_type_all_protocol_sacn_mode_unicast = gensym("unicast");
    s_lx_common_syms.s_type_input_protocol_dmxusbpro_mode_input = gensym("input");
    s_lx_common_syms.s_type_output_protocol_dmxusbpro_mode_output = gensym("output");
    s_lx_common_syms.s_type_output_protocol_opendmxusb_mode_output = gensym("output");

    s_lx_common_syms.s_interface_none = gensym("[none]");
    s_lx_common_syms.s_interface_ip_adapter_global = gensym("[global]");
    s_lx_common_syms.s_interface_ip_adapter_all = gensym("All (0.0.0.0)");

    s_lx_common_syms.s_ip_address_all = gensym("0.0.0.0");
    s_lx_common_syms.s_ip_address_broadcast = gensym("255.255.255.255");

    s_lx_common_syms.s_service_notify_universes_updated = gensym("universes_updated");

	s_lx_common_syms.s_universe_types = gensym("universe_types");
	s_lx_common_syms.s_universe_protocols = gensym("universe_protocols");
	s_lx_common_syms.s_universe_protocol_modes = gensym("universe_protocol_modes");
	s_lx_common_syms.s_universe_protocol_interfaces = gensym("universe_protocol_interfaces");

    return &s_lx_common_syms;
}