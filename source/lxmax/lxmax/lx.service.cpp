// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h> 
#include <ext_obex.h>
#include <ext_globalsymbol.h>
#include <ext_dictobj.h>

#include "lxmax.h"



t_class* lxmax_write_class;

void* lxmax_service_new(t_symbol* s, long argc, t_atom* argv);
void lxmax_service_free(t_lx_service* x);

t_max_err lxmax_service_notify(t_lx_service* x, t_symbol* s, t_symbol* msg, void* sender, void* data);

t_max_err lxmax_service_preferences_load(t_lx_service* x);
t_max_err lxmax_service_preferences_save(t_lx_service* x);
void lxmax_service_preferences_clear(t_lx_service* x);



void ext_main(void* r)
{
	common_symbols_init();
	lx_sym_init();

	t_class* c;

	c = class_new(LX_SERVICE_CLASS_NAME, (method)lxmax_service_new, (method)lxmax_service_free, (long)sizeof(t_lx_service),
		0L, A_GIMME, 0);

	class_addmethod(c, (method)lxmax_service_notify, "notify", A_CANT, 0);

	CLASS_ATTR_SYM(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, t_lx_service, global_artnet_interface);

	CLASS_ATTR_SYM_VARSIZE(c, LX_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, t_lx_service, global_artnet_unicast_addresses,
		global_artnet_unicast_addresses_count, LX_UNICAST_ADDRESSES_MAX);

	CLASS_ATTR_CHAR(c, LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, t_lx_service, artnet_sync_packets_enable);
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, 1);

	CLASS_ATTR_SYM(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, t_lx_service, global_sacn_interface);

	CLASS_ATTR_SYM_VARSIZE(c, LX_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, t_lx_service, global_sacn_unicast_addresses,
		global_sacn_unicast_addresses_count, LX_UNICAST_ADDRESSES_MAX);

	CLASS_ATTR_CHAR(c, LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, t_lx_service, sacn_sync_packets_enable);
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, 1);

	CLASS_ATTR_LONG(c, LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, t_lx_service, sacn_sync_address);
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, LX_UNIVERSE_SACN_MIN, LX_UNIVERSE_SACN_MAX);

	CLASS_ATTR_DOUBLE(c, LX_GLOBAL_CONFIG_FRAMERATE, 0, t_lx_service, framerate);
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, LX_DMX_FRAMERATE_MIN, LX_DMX_FRAMERATE_MAX);

	CLASS_ATTR_CHAR(c, LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, t_lx_service, allow_nondmx_framerate)
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, 1);

	CLASS_ATTR_CHAR(c, LX_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, t_lx_service, force_framerate);
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, 1);

	CLASS_ATTR_CHAR(c, LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, t_lx_service, output_empty_universes);
	CLASS_ATTR_FILTER_CLIP(c, LX_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, 1);

	


	class_register(CLASS_NOBOX, c);
	lxmax_write_class = c;

	t_object* instance = (t_object*)object_new_typed(CLASS_NOBOX, _lxsym_lxmax, 0, nullptr);
	globalsymbol_bind(instance, LX_SERVICE_SYMBOL_NAME, 0);
}


void* lxmax_service_new(t_symbol* s, long argc, t_atom* argv)
{
	t_lx_service* x = (t_lx_service*)object_alloc(lxmax_write_class);
	if (x == nullptr)
		return nullptr;

#ifdef _DEBUG
	object_post((t_object*)x, "V%ld.%ld.%ld - DEBUG - %s", LXMAX_VERSION_MAJOR, LXMAX_VERSION_MINOR, LXMAX_VERSION_BUGFIX, LXMAX_COPYRIGHT);
#else
	object_post((t_object*)x, "V%ld.%ld.%ld - %s", LXMAX_VERSION_MAJOR, LXMAX_VERSION_MINOR, LXMAX_VERSION_BUGFIX, LXMAX_COPYRIGHT);
#endif

	x->network_adapters = lx_network_adapter_enumerate();
	x->universes = linklist_new();

	lxmax_service_preferences_clear(x);
	lxmax_service_preferences_load(x);
	lxmax_service_preferences_save(x);

	return x;
}

void lxmax_service_free(t_lx_service* x)
{
	object_free(x->universes);
	object_free(x->network_adapters);
}

t_max_err lxmax_service_notify(t_lx_service* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
	/*if (sender == x->dict_preferences && msg == _sym_modified)
	{
		lxmax_preferences_validate_and_fix(x, x->dict_preferences);
		lxmax_preferences_save(x, x->dict_preferences);
	}*/

	return MAX_ERR_NONE;
}

t_max_err lxmax_service_preferences_load(t_lx_service* x)
{
	short max_pref_path;
	preferences_path(nullptr, true, &max_pref_path);

	t_dictionary* dict_preferences;

	if (dictionary_read(LX_PREFERENCES_FILE_NAME, max_pref_path, &dict_preferences) != MAX_ERR_NONE)
		return MAX_ERR_GENERIC;

	t_atomarray* atomarray_universes;
	if (dictionary_getatomarray(dict_preferences, _lxsym_universes, (t_object**)&atomarray_universes) == MAX_ERR_NONE)
	{
		linklist_clear(x->universes);

		long argc;
		t_atom* argv;
		atomarray_getatoms(atomarray_universes, &argc, &argv);

		for(long i = 0; i < argc; ++i)
		{
			t_dictionary* dict_universe = (t_dictionary*)atom_getobj(argv + i);
			linklist_append(x->universes, lx_universe_config_new(x, dict_universe));
		}
	}

	return MAX_ERR_NONE;
}

t_max_err lxmax_service_preferences_save(t_lx_service* x)
{
	t_dictionary* dict_preferences = dictionary_new();

	t_atomarray* atomarray_universes = atomarray_new(0, nullptr);

	t_lx_universe_config* config = (t_lx_universe_config*)linklist_getindex(x->universes, 0);
	while(config)
	{
		t_dictionary* dict_universe = dictionary_new();
		lx_universe_config_to_dictionary(config, dict_universe);

		t_atom value;
		atom_setobj(&value, dict_universe);
		atomarray_appendatom(atomarray_universes, &value);

		linklist_next(x->universes, config, (void**)&config);
	}

	dictionary_appendatomarray(dict_preferences, _lxsym_universes, (t_object*)atomarray_universes);

	short max_pref_path;
	preferences_path(nullptr, true, &max_pref_path);

	const t_max_err result = dictionary_write(dict_preferences, LX_PREFERENCES_FILE_NAME, max_pref_path);

	object_free(dict_preferences);

	return result;
}

void lxmax_service_preferences_clear(t_lx_service* x)
{
	x->global_artnet_interface = _lxsym_interface_ip_adapter_all;
	memset(x->global_artnet_unicast_addresses, 0, LX_UNICAST_ADDRESSES_MAX);
	x->global_artnet_unicast_addresses_count = 0;
	x->artnet_sync_packets_enable = true;
	x->global_sacn_interface = _lxsym_interface_ip_adapter_all;
	memset(x->global_sacn_unicast_addresses, 0, LX_UNICAST_ADDRESSES_MAX);
	x->global_sacn_unicast_addresses_count = 0;
	x->sacn_sync_packets_enable = true;
	x->sacn_sync_address = LX_UNIVERSE_SACN_MIN;
	x->framerate = LX_DMX_FRAMERATE_MAX;
	x->allow_nondmx_framerate = false;
	x->force_framerate = false;
	x->output_empty_universes = false;
	

	linklist_clear(x->universes);

	linklist_append(x->universes, lx_universe_config_new(x, nullptr));
}