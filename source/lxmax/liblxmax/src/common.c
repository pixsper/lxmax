// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "common.h"

#include "net.h"
#include "universe_config.h"

t_symbol* _lxsym_lxmax_service;
t_symbol* _lxsym_lxmax;
t_symbol* _lxsym_address;
t_symbol* _lxsym_addresses;
t_symbol* _lxsym_subnet_mask;
t_symbol* _lxsym_universes;
t_symbol* _lxsym_not_applicable;

t_symbol* _lxsym_universe_config_label;
t_symbol* _lxsym_universe_config_type;
t_symbol* _lxsym_universe_config_enable;
t_symbol* _lxsym_universe_config_protocol;
t_symbol* _lxsym_universe_config_protocol_mode;
t_symbol* _lxsym_universe_config_protocol_interface;
t_symbol* _lxsym_universe_config_internal_universe;
t_symbol* _lxsym_universe_config_protocol_universe;
t_symbol* _lxsym_universe_config_priority;
t_symbol* _lxsym_universe_config_unicast_addresses;

t_symbol* _lxsym_global_config_global_artnet_interface;
t_symbol* _lxsym_global_config_global_artnet_unicast_addresses;
t_symbol* _lxsym_global_config_artnet_sync_packets_enable;
t_symbol* _lxsym_global_config_global_sacn_interface;
t_symbol* _lxsym_global_config_global_sacn_unicast_addresses;
t_symbol* _lxsym_global_config_sacn_sync_packets_enable;
t_symbol* _lxsym_global_config_sacn_sync_address;
t_symbol* _lxsym_global_config_framerate;
t_symbol* _lxsym_global_config_allow_non_dmx_framerate;
t_symbol* _lxsym_global_config_force_framerate;
t_symbol* _lxsym_global_config_output_empty_universes;

t_symbol* _lxsym_type_input;
t_symbol* _lxsym_type_output;

t_symbol* _lxsym_protocol_artnet;
t_symbol* _lxsym_protocol_sacn;

t_symbol* _lxsym_type_input_protocol_artnet_mode_input;
t_symbol* _lxsym_type_output_protocol_artnet_mode_broadcast;
t_symbol* _lxsym_type_output_protocol_artnet_mode_unicast;
t_symbol* _lxsym_type_all_protocol_sacn_mode_multicast;
t_symbol* _lxsym_type_all_protocol_sacn_mode_unicast;

t_symbol* _lxsym_interface_ip_adapter_global;
t_symbol* _lxsym_interface_ip_adapter_all;

t_symbol* _lxsym_ip_address_all;
t_symbol* _lxsym_ip_address_broadcast;

t_symbol* _lxsym_service_notify_universes_updated;



static bool g_is_lxmax_init = false;

void lx_sym_init()
{
	if (!g_is_lxmax_init)
	{
		common_symbols_init();

		_lxsym_lxmax_service = gensym(LX_SERVICE_SYMBOL_NAME);
		_lxsym_lxmax = gensym(LX_SERVICE_CLASS_NAME);

		_lxsym_address = gensym("address");
		_lxsym_addresses = gensym("addresses");
		_lxsym_subnet_mask = gensym("subnet_mask");
		_lxsym_universes = gensym("universes");
		_lxsym_not_applicable = gensym("n/a");

		_lxsym_universe_config_label = gensym(LX_UNIVERSE_CONFIG_LABEL_SYMBOL_NAME);
		_lxsym_universe_config_type = gensym(LX_UNIVERSE_CONFIG_TYPE_SYMBOL_NAME);
		_lxsym_universe_config_enable = gensym(LX_UNIVERSE_CONFIG_ENABLE_SYMBOL_NAME);
		_lxsym_universe_config_protocol = gensym(LX_UNIVERSE_CONFIG_PROTOCOL_SYMBOL_NAME);
		_lxsym_universe_config_protocol_mode = gensym(LX_UNIVERSE_CONFIG_PROTOCOL_MODE_SYMBOL_NAME);
		_lxsym_universe_config_protocol_interface = gensym(LX_UNIVERSE_CONFIG_PROTOCOL_INTERFACE_SYMBOL_NAME);
		_lxsym_universe_config_internal_universe = gensym(LX_UNIVERSE_CONFIG_INTERNAL_UNIVERSE_SYMBOL_NAME);
		_lxsym_universe_config_protocol_universe = gensym(LX_UNIVERSE_CONFIG_PROTOCOL_UNIVERSE_SYMBOL_NAME);
		_lxsym_universe_config_priority = gensym(LX_UNIVERSE_CONFIG_PRIORITY_SYMBOL_NAME);
		_lxsym_universe_config_unicast_addresses = gensym(LX_UNIVERSE_CONFIG_UNICAST_ADDRESSES_SYMBOL_NAME);

		_lxsym_global_config_global_artnet_interface = gensym(LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE);
		_lxsym_global_config_global_artnet_unicast_addresses = gensym(LX_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES);
		_lxsym_global_config_artnet_sync_packets_enable = gensym(LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE);
		_lxsym_global_config_global_sacn_interface = gensym(LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE);
		_lxsym_global_config_global_sacn_unicast_addresses = gensym(LX_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES);
		_lxsym_global_config_sacn_sync_packets_enable = gensym(LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE);
		_lxsym_global_config_sacn_sync_address = gensym(LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS);
		_lxsym_global_config_framerate = gensym(LX_GLOBAL_CONFIG_FRAMERATE);
		_lxsym_global_config_allow_non_dmx_framerate = gensym(LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE);
		_lxsym_global_config_force_framerate = gensym(LX_GLOBAL_CONFIG_FORCE_FRAMERATE);
		_lxsym_global_config_output_empty_universes = gensym(LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES);

		_lxsym_type_input = gensym("input");
		_lxsym_type_output = gensym("output");

		_lxsym_protocol_artnet = gensym("Art-Net");
		_lxsym_protocol_sacn = gensym("sACN");

		_lxsym_type_input_protocol_artnet_mode_input= gensym("input");
		_lxsym_type_output_protocol_artnet_mode_broadcast = gensym("broadcast");
		_lxsym_type_output_protocol_artnet_mode_unicast = gensym("unicast");
		_lxsym_type_all_protocol_sacn_mode_multicast = gensym("multicast");
		_lxsym_type_all_protocol_sacn_mode_unicast = gensym("unicast");

		_lxsym_interface_ip_adapter_global = gensym("[global]");
		_lxsym_interface_ip_adapter_all = gensym("All (0.0.0.0)");

		_lxsym_ip_address_all = gensym("0.0.0.0");
		_lxsym_ip_address_broadcast = gensym("255.255.255.255");

		_lxsym_service_notify_universes_updated = gensym("universes_updated");

		lx_network_adapter_register_class();
		lx_universe_config_register_class();

		g_is_lxmax_init = true;
	}
}