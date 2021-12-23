// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h> 
#include <ext_obex.h>
#include <ext_dictionary.h>
#include <ext_dictobj.h>
#include <ext_globalsymbol.h>

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
void lx_config_remove_universe(t_lx_config* x, t_atom_long index);
void lx_config_clear_universes(t_lx_config* x);
void lx_config_quick_patch(t_lx_config* x, t_symbol *s, long argc, t_atom* argv);

t_max_err lx_config_attr_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_config_attr_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv);


void ext_main(void* r)
{
	common_symbols_init();
	lx_sym_init();

	t_class* c;

	c = class_new("lx.config", (method)lx_config_new, (method)lx_config_free, (long)sizeof(t_lx_config),
	0L, A_GIMME, 0);

	class_addmethod(c, (method)lx_config_assist, "assist", A_CANT, 0);

	class_addmethod(c, (method)lx_config_add_universe, "add_universe", A_NOTHING, 0);
	class_addmethod(c, (method)lx_config_remove_universe, "remove_universe", A_LONG, 0);
	class_addmethod(c, (method)lx_config_clear_universes, "clear_universes", A_NOTHING, 0);
	class_addmethod(c, (method)lx_config_quick_patch, "quick_patch", A_GIMME, 0);


	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 
		_sym_symbol, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "Global Art-Net Network Interface");
	CLASS_ATTR_ENUM(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "\"All (0.0.0.0)\"");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "1");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, "\"LXMax - Art-Net\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 
		_sym_symbol, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, "Global Art-Net Unicast Addresses");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, "2");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, "\"LXMax - Art-Net\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 
		_sym_char, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, "onoff", "Enable Art-Net Sync Packets");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, "3");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, "\"LXMax - Art-Net\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 
		_sym_symbol, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "Global sACN Network Interface");
	CLASS_ATTR_ENUM(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "\"All (0.0.0.0)\"");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "1");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 
		_sym_symbol, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_LABEL(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, "Global sACN Unicast Addresses");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, "2");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 
		_sym_char, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, "onoff", "Enable sACN Sync Packets");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, "3");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 
		_sym_long, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, LX_UNIVERSE_SACN_MIN, LX_UNIVERSE_SACN_MAX);
	CLASS_ATTR_LABEL(c, LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, "sACN Sync Address");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, "4");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, "\"LXMax - sACN\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_FRAMERATE, 
		_sym_float64, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_FRAMERATE, LX_DMX_FRAMERATE_MIN, LX_DMX_FRAMERATE_MAX);
	CLASS_ATTR_LABEL(c, LX_GLOBAL_CONFIG_FRAMERATE, 0, "Output Framerate");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_FRAMERATE, 0, "1");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_FRAMERATE, 0, "\"LXMax - Output\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 
		_sym_char, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, "onoff", "Allow Non-DMX Framerates");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, "2");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, "\"LXMax - Output\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_FORCE_FRAMERATE, 
		_sym_char, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LX_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, "onoff", "Force Output At Framerate");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, "3");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, "\"LXMax - Output\"");

	class_addattr(c, attribute_new(LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 
		_sym_char, 0, lx_config_attr_get, lx_config_attr_set));
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, 1);
	CLASS_ATTR_STYLE_LABEL(c, LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, "onoff", "Output Empty Universes");
	CLASS_ATTR_ORDER(c, LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, "4");
	CLASS_ATTR_CATEGORY(c, LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, "\"LXMax - Output\"");

	

	

	



	class_register(CLASS_BOX, c);
	lx_config_class = c;
}




void* lx_config_new(t_symbol* s, long argc, t_atom* argv)
{
	t_lx_config* x = (t_lx_config*)object_alloc(lx_config_class);
	if (x == NULL)
		return NULL;

	x->service = (t_lx_service*)globalsymbol_reference((t_object*)x, LX_SERVICE_SYMBOL_NAME, LX_SERVICE_CLASS_NAME);
	if (x->service == NULL)
	{
		object_error((t_object*)x, "Failed to find active LXMax service");
		object_free(x);
		return NULL;
	}

	x->outlet_dump = outlet_new(x, NULL);

	return x;
}

void lx_config_free(t_lx_config* x)
{
	globalsymbol_dereference((t_object*)x, LX_SERVICE_SYMBOL_NAME, LX_SERVICE_CLASS_NAME);
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
	lxmax_service_universes_add(x->service, lx_universe_config_new(x->service, NULL), -1);
}

void lx_config_remove_universe(t_lx_config* x, t_atom_long index)
{
	t_lx_universe_config* config = linklist_getindex(x->service->universes, index);
	if (config)
		lxmax_service_universes_remove(x->service, config);
}

void lx_config_clear_universes(t_lx_config* x)
{
	lxmax_service_universes_clear(x->service);
}

void lx_config_quick_patch(t_lx_config* x, t_symbol* s, long argc, t_atom* argv)
{
	
}



t_max_err lx_config_attr_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv)
{
	t_symbol* attr_name = (t_symbol*)object_method((t_object *)attr, _sym_getname);
	object_attr_getvalueof(x->service, attr_name, argc, argv);
}

t_max_err lx_config_attr_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	t_symbol* attr_name = (t_symbol*)object_method((t_object *)attr, _sym_getname);
	object_attr_setvalueof(x->service, attr_name, argc, argv);
}