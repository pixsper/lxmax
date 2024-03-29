// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "lx_common.h"

#include <ext_dictionary.h>

#include "lx_dmx.h"
#include "lx_net.h"
#include "lx_serial.h"


BEGIN_USING_C_LINKAGE

typedef enum _lx_universe_type
{
	LX_UNIVERSE_TYPE_NONE,
	LX_UNIVERSE_TYPE_INPUT,
	LX_UNIVERSE_TYPE_OUTPUT,
	LX_UNIVERSE_TYPE_COUNT
	
} e_lx_universe_type;

typedef enum _lx_universe_protocol
{
	LX_UNIVERSE_PROTOCOL_NONE,
	LX_UNIVERSE_PROTOCOL_ARTNET,
	LX_UNIVERSE_PROTOCOL_SACN,
    LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO,
    LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB,
	LX_UNIVERSE_PROTOCOL_COUNT

} e_lx_universe_protocol;

typedef struct _lx_service t_lx_service;

typedef struct _lx_universe_config
{
	t_object object;
	t_object* registered_object;
	t_lx_service* service;

	t_symbol* name;

	t_lx_network_adapter* adapter;
    t_lx_serial_port* serial_port;

	t_symbol* label;
	e_lx_universe_type type;
	t_bool enable;
	e_lx_universe_protocol protocol;
	t_symbol* protocol_mode;
	t_symbol* protocol_interface;
	long internal_universe;
	long protocol_universe;
	long priority;
	t_lx_ip_address unicast_addresses[LX_UNICAST_ADDRESSES_MAX];
	long unicast_addresses_count;

	t_symbol* status;

} t_lx_universe_config;

extern t_class* lx_universe_config_class;

DLL_API t_symbol* lx_universe_type_to_sym(e_lx_universe_type type);
DLL_API t_max_err lx_universe_type_from_sym(const t_symbol* s, e_lx_universe_type* type);
DLL_API void lx_universe_type_get_list(e_lx_universe_protocol protocol, long* argc, t_atom** argv);

DLL_API t_symbol* lx_universe_protocol_to_sym(e_lx_universe_protocol protocol);
DLL_API t_max_err lx_universe_protocol_from_sym(const t_symbol* s, e_lx_universe_protocol* protocol);
DLL_API void lx_universe_protocol_get_list(long* argc, t_atom** argv);
DLL_API void lx_universe_protocol_mode_get_list(e_lx_universe_type type, e_lx_universe_protocol protocol,
												long* argc, t_atom** argv);
DLL_API void lx_universe_protocol_interface_get_list(t_lx_service* service, e_lx_universe_protocol protocol,
													 long* argc, t_atom** argv);

void lx_universe_config_register_class();
DLL_API t_lx_universe_config* lx_universe_config_new(t_lx_service* service, t_dictionary* dict);
DLL_API t_lx_universe_config* lx_universe_config_clone(t_lx_service* service, t_lx_universe_config* source);
DLL_API void lx_universe_config_free(t_lx_universe_config* x);

DLL_API void lx_universe_config_increment(t_lx_universe_config* x, long count);

DLL_API void lx_universe_config_find_interface(t_lx_universe_config* x);

DLL_API void lx_universe_config_to_dictionary(const t_lx_universe_config* x, t_dictionary* dict);

t_max_err lx_universe_type_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_universe_type_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_universe_protocol_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_universe_protocol_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_universe_protocol_mode_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_universe_protocol_interface_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_universe_protocol_universe_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_universe_protocol_universe_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_universe_priority_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_universe_priority_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_universe_unicast_addresses_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_universe_unicast_addresses_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);

END_USING_C_LINKAGE
