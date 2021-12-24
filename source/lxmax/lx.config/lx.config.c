// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h> 
#include <ext_obex.h>

#include "lxmax.h"

typedef struct t_lx_config
{
	t_object object;
	t_lx_service* service;

	t_outlet* outlet_dump;

} t_lx_config;

static t_class* lx_config_class;

void* lx_config_new(t_symbol* s, long argc, t_atom* argv);
void lx_config_free(t_lx_config* x);
void lx_config_assist(t_lx_config* x, void* b, long m, long a, char* s);

void lx_config_add_universe(t_lx_config* x);
void lx_config_remove_universe(t_lx_config* x, t_symbol* name);
void lx_config_clear_universes(t_lx_config* x);
void lx_config_quick_patch(t_lx_config* x, t_symbol* s, long argc, t_atom* argv);

void lx_config_get_universe_types(t_lx_config* x, t_symbol* protocol);
void lx_config_get_universe_protocols(t_lx_config* x);
void lx_config_get_universe_protocol_modes(t_lx_config* x, t_symbol* type, t_symbol* protocol);
void lx_config_get_universe_protocol_interfaces(t_lx_config* x, t_symbol* protocol);

t_max_err lx_config_attr_get(t_lx_config* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_config_attr_set(t_lx_config* x, t_object* attr, long argc, t_atom* argv);


void ext_main(void* r)
{
	common_symbols_init();
    lx_common_symbols_init();

	t_class* c;

	c = class_new("lx.config", (method)lx_config_new, (method)lx_config_free, (long)sizeof(t_lx_config),
	0L, A_GIMME, 0);

	class_addmethod(c, (method)lx_config_assist, "assist", A_CANT, 0);

	class_addmethod(c, (method)lx_config_add_universe, "add_universe", A_NOTHING, 0);
	class_addmethod(c, (method)lx_config_remove_universe, "remove_universe", A_LONG, 0);
	class_addmethod(c, (method)lx_config_clear_universes, "clear_universes", A_NOTHING, 0);
	class_addmethod(c, (method)lx_config_quick_patch, "quick_patch", A_GIMME, 0);

	class_addmethod(c, (method)lx_config_get_universe_types, "get_universe_types", 0);
	class_addmethod(c, (method)lx_config_get_universe_protocols, "get_universe_protocols", 0);
	class_addmethod(c, (method)lx_config_get_universe_protocol_modes, "get_universe_protocol_modes", A_SYM, A_SYM, 0);
	class_addmethod(c, (method)lx_config_get_universe_protocol_interfaces, "get_universe_protocol_interfaces", A_SYM, 0);


	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE,
                                   _sym_symbol, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "Global Art-Net Network Interface");
	CLASS_ATTR_ENUM(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "1");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "\"LXMax - Art-Net\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES,
                                   _sym_symbol, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, "Global Art-Net Unicast Addresses");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, "2");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, "\"LXMax - Art-Net\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE,
								   _sym_char, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, "onoff", "Enable Art-Net Sync Packets");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, "3");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, "\"LXMax - Art-Net\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE,
								   _sym_symbol, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "Global sACN Network Interface");
	CLASS_ATTR_ENUM(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "1");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES,
								   _sym_symbol, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, "Global sACN Unicast Addresses");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, "2");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE,
								   _sym_char, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, "onoff", "Enable sACN Sync Packets");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, "3");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS,
								   _sym_long, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, LX_UNIVERSE_SACN_MIN, LX_UNIVERSE_SACN_MAX);
	CLASS_ATTR_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, "sACN Sync Address");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, "4");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE,
	                               _sym_float64, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE, LX_DMX_FRAMERATE_MIN, LX_DMX_FRAMERATE_MAX);
	CLASS_ATTR_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE, 0, "Output Framerate");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE, 0, "1");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE, 0, "\"LXMax - Output\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE,
								   _sym_char, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, "onoff", "Allow Non-DMX Framerates");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, "2");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, "\"LXMax - Output\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE,
								   _sym_char, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, "onoff", "Force Output At Framerate");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, "3");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, "\"LXMax - Output\"");

	class_addattr(c, attribute_new(LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES,
								   _sym_char, 0, (method)lx_config_attr_get, (method)lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, "onoff", "Output Empty Universes");
	CLASS_ATTR_ORDER(c, LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, "4");
	CLASS_ATTR_CATEGORY(c, LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, "\"LXMax - Output\"");


	class_register(CLASS_BOX, c);
	lx_config_class = c;
}




void* lx_config_new(t_symbol* s, long argc, t_atom* argv)
{
	t_lx_config* x = (t_lx_config*)object_alloc(lx_config_class);
	if (x == NULL)
		return NULL;

	x->service = lx_service_subscribe((t_object*)x);
	if (!x->service)
	{
		object_free(x);
		return NULL;
	}

	x->outlet_dump = outlet_new(x, NULL);

	long ac = 0;
	t_atom* av = NULL;
	lx_network_adapter_get_menu_items(x->service->network_adapters, &ac, &av);
	object_attr_attr_setvalueof((t_object*)x, _lxsym_global_config_global_artnet_interface, _sym_enumvals, ac - 1, av + 1);
	object_attr_attr_setvalueof((t_object*)x, _lxsym_global_config_global_sacn_interface, _sym_enumvals, ac - 1, av + 1);

	if (av)
		sysmem_freeptr(av);

	return x;
}

void lx_config_free(t_lx_config* x)
{
	if (x->service)
		lx_service_unsubscribe((t_object*)x, x->service);
}

void lx_config_assist(t_lx_config* x, void* b, long m, long a, char* s)
{
	if (m == ASSIST_INLET)
	{
		sprintf(s, "I am inlet %ld", a);
	}
	else
	{
		sprintf(s, "I am outlet %ld", a);
	}
}


void lx_config_add_universe(t_lx_config* x)
{
	lx_service_universes_add(x->service, lx_universe_config_new(x->service, NULL));
}

void lx_config_remove_universe(t_lx_config* x, t_symbol* name)
{
	t_lx_universe_config* config = NULL;
	if (hashtab_lookup(x->service->universes, name, (t_object**)&config) == MAX_ERR_NONE)
		lx_service_universes_remove(x->service, config);
}

void lx_config_clear_universes(t_lx_config* x)
{
	lx_service_universes_clear(x->service);
}

void lx_config_quick_patch(t_lx_config* x, t_symbol* s, long argc, t_atom* argv)
{
	if (argc < 8)
	{
		object_error((t_object*)x, "Incorrect number of arguments for quick_patch");
		return;
	}

	if ((atom_gettype(argv) != A_LONG && atom_gettype(argv) != A_FLOAT)
		|| (atom_gettype(argv + 1) != A_LONG && atom_gettype(argv + 1) != A_FLOAT)
		|| atom_gettype(argv + 2) != A_SYM || atom_gettype(argv + 3) != A_SYM
		|| atom_gettype(argv + 4) != A_SYM || atom_gettype(argv + 5) != A_SYM
		|| (atom_gettype(argv + 6) != A_LONG && atom_gettype(argv + 6) != A_FLOAT)
	   || (atom_gettype(argv + 7) != A_LONG && atom_gettype(argv + 7) != A_FLOAT))
	{
		object_error((t_object*)x, "Incorrect argument type(s) for quick_patch");
		return;
	}

	t_atom_long is_clear = atom_getlong(argv);
	t_atom_long patch_count = atom_getlong(argv + 1);
	t_symbol* type = atom_getsym(argv + 2);
	t_symbol* protocol = atom_getsym(argv + 3);
	t_symbol* protocol_mode = atom_getsym(argv + 4);
	t_symbol* protocol_interface = atom_getsym(argv + 5);
	t_atom_long start_internal_universe = atom_getlong(argv + 6);
	t_atom_long start_protocol_universe = atom_getlong(argv + 7);

	e_lx_universe_type type_value;
	if (lx_universe_type_from_sym(type, &type_value) != MAX_ERR_NONE)
	{
		object_error((t_object*)x, "Invalid universe type value '%s'", type->s_name);
		return;
	}

	e_lx_universe_protocol protocol_value;
	if (lx_universe_protocol_from_sym(protocol, &protocol_value) != MAX_ERR_NONE)
	{
		object_error((t_object*)x, "Invalid universe protocol value '%s'", protocol->s_name);
		return;
	}

	long ac = 0;
	t_atom* av = NULL;

	t_symbol* protocol_mode_value = NULL;
	lx_universe_protocol_mode_get_list(type_value, protocol_value, &ac, &av);
	for(long i = 0; i < ac; ++i)
	{
		if (atom_getsym(av + i) == protocol_mode)
		{
			protocol_mode_value = protocol_mode;
			break;
		}
	}
	sysmem_freeptr(av);

	if (!protocol_mode_value)
	{
		object_error((t_object*)x, "Invalid universe protocol mode value '%s'", protocol_mode->s_name);
		sysmem_freeptr(argv);
		return;
	}

	ac = 0;
	t_symbol* protocol_interface_value = NULL;
	lx_universe_protocol_interface_get_list(x->service, protocol_value, &ac, &av);
	for(long i = 0; i < ac; ++i)
	{
		if (atom_getsym(av + i) == protocol_interface)
		{
			protocol_interface_value = protocol_interface;
			break;
		}
	}
	sysmem_freeptr(av);

	if (!protocol_interface_value)
	{
		object_error((t_object*)x, "Invalid universe protocol mode value '%s'", protocol_mode->s_name);
		return;
	}

	if (is_clear > 0)
		lx_service_universes_clear(x->service);

	lx_service_universes_quick_patch(x->service, patch_count, type_value, protocol_value,
									 protocol_mode_value, protocol_interface_value,
									 start_internal_universe, start_protocol_universe);
}

void lx_config_get_universe_types( t_lx_config* x, t_symbol* protocol)
{
	e_lx_universe_protocol protocol_value;
	if (lx_universe_protocol_from_sym(protocol, &protocol_value) == MAX_ERR_NONE)
	{
		long argc = 0;
		t_atom* argv = NULL;
		lx_universe_type_get_list(protocol_value, &argc, &argv);
		outlet_anything(x->outlet_dump, _lxsym_universe_types, argc, argv);
	}
	else
	{
		object_error((t_object*)x, "Invalid protocol name");
	}
}

void lx_config_get_universe_protocols(t_lx_config* x)
{
	long argc = 0;
	t_atom* argv = NULL;
	lx_universe_protocol_get_list(&argc, &argv);
	outlet_anything(x->outlet_dump, _lxsym_universe_protocols, argc, argv);
}

void lx_config_get_universe_protocol_modes(t_lx_config* x, t_symbol* type, t_symbol* protocol)
{
	e_lx_universe_type type_value;
	if (lx_universe_type_from_sym(type, &type_value) != MAX_ERR_NONE)
	{
		object_error((t_object*)x, "Invalid universe type value '%s'", type->s_name);
		return;
	}

	e_lx_universe_protocol protocol_value;
	if (lx_universe_protocol_from_sym(protocol, &protocol_value) != MAX_ERR_NONE)
	{
		object_error((t_object*)x, "Invalid universe protocol value '%s'", protocol->s_name);
		return;
	}

	long argc = 0;
	t_atom* argv = NULL;
	lx_universe_protocol_mode_get_list(type_value, protocol_value, &argc, &argv);
	outlet_anything(x->outlet_dump, _lxsym_universe_protocol_modes, argc, argv);
}

void lx_config_get_universe_protocol_interfaces(t_lx_config* x, t_symbol* protocol)
{
	e_lx_universe_protocol protocol_value;
	if (lx_universe_protocol_from_sym(protocol, &protocol_value) != MAX_ERR_NONE)
	{
		object_error((t_object*)x, "Invalid universe protocol value '%s'", protocol->s_name);
		return;
	}

	long argc = 0;
	t_atom* argv = NULL;
	lx_universe_protocol_interface_get_list(x->service, protocol_value, &argc, &argv);
	outlet_anything(x->outlet_dump, _lxsym_universe_protocol_interfaces, argc, argv);
}



t_max_err lx_config_attr_get(t_lx_config* x, t_object* attr, long* argc, t_atom** argv)
{
	t_symbol* attr_name = (t_symbol*)object_method((t_object *)attr, _sym_getname);
	return object_attr_getvalueof(x->service, attr_name, argc, argv);
}

t_max_err lx_config_attr_set(t_lx_config* x, t_object* attr, long argc, t_atom* argv)
{
	t_symbol* attr_name = (t_symbol*)object_method((t_object *)attr, _sym_getname);
	return object_attr_setvalueof(x->service, attr_name, argc, argv);
}