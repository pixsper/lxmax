// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h> 
#include <ext_obex.h>

#include "lxmax.h"

#include "internals/dmx_io_service.hpp"

static t_class* lxmax_service_class;
static t_object* s_lxmax_service_instance;

void lx_onquit(void* a);

void* lx_service_new();
void lx_service_free(t_lx_service* x);

t_max_err lx_service_notify(t_lx_service* x, t_symbol* s, t_symbol* msg, void* sender, void* data);

void lx_service_qelem_save(t_lx_service* x);

t_max_err lx_service_preferences_load(t_lx_service* x);
t_max_err lx_service_preferences_save(t_lx_service* x);
void lx_service_preferences_clear(t_lx_service* x);

void lx_service_find_global_artnet_interface(t_lx_service* x);
void lx_service_find_global_sacn_interface(t_lx_service* x);

void lx_service_io_thread(t_lx_service* x);

void lx_service_io_clock_tick(t_lx_service* x);

t_max_err lx_service_global_artnet_interface_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_service_global_artnet_unicast_addresses_get(t_lx_service* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_service_global_artnet_unicast_addresses_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_service_global_sacn_interface_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv);
t_max_err lx_service_global_sacn_unicast_addresses_get(t_lx_service* x, t_object* attr, long* argc, t_atom** argv);
t_max_err lx_service_global_sacn_unicast_addresses_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv);

void ext_main(void* r)
{
	common_symbols_init();
	lx_common_symbols_init();

	t_class* c;

	c = class_new(LX_CLASSNAME_SERVICE, (method)lx_service_new,(method)lx_service_free,
				  (long)sizeof(t_lx_service),0L, 0);

	class_addmethod(c, (method)lx_service_notify, "notify", A_CANT, 0);

	CLASS_ATTR_SYM(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE, 0, t_lx_service, global_artnet_interface);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_INTERFACE,
	                     NULL, (method)lx_service_global_artnet_interface_set);

	CLASS_ATTR_SYM_VARSIZE(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES, 0, t_lx_service,
						   global_artnet_unicast_addresses, global_artnet_unicast_addresses_count,
						   LX_UNICAST_ADDRESSES_MAX);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_ARTNET_UNICAST_ADDRESSES,
	                     (method)lx_service_global_artnet_unicast_addresses_get,
						 (method)lx_service_global_artnet_unicast_addresses_set);

	CLASS_ATTR_CHAR(c, LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, t_lx_service, artnet_sync_packets_enable);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_ARTNET_SYNC_PACKETS_ENABLE, 0, 1);

	CLASS_ATTR_SYM(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE, 0, t_lx_service, global_sacn_interface);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_INTERFACE,
	                     NULL, (method)lx_service_global_sacn_interface_set);

	CLASS_ATTR_SYM_VARSIZE(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES, 0, t_lx_service,
						   global_sacn_unicast_addresses, global_sacn_unicast_addresses_count,
						   LX_UNICAST_ADDRESSES_MAX);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_GLOBAL_CONFIG_GLOBAL_SACN_UNICAST_ADDRESSES,
	                     (method)lx_service_global_sacn_unicast_addresses_get,
	                     (method)lx_service_global_sacn_unicast_addresses_set);

	CLASS_ATTR_CHAR(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, t_lx_service, sacn_sync_packets_enable);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_PACKETS_ENABLE, 0, 1);

	CLASS_ATTR_LONG(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, 0, t_lx_service, sacn_sync_address);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_SACN_SYNC_ADDRESS, LX_UNIVERSE_SACN_MIN, LX_UNIVERSE_SACN_MAX);

	CLASS_ATTR_DOUBLE(c, LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE, 0, t_lx_service, max_framerate);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_MAX_FRAMERATE,
						   LX_DMX_FRAMERATE_MIN, LX_DMX_FRAMERATE_MAX);

	CLASS_ATTR_CHAR(c, LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, t_lx_service, allow_nondmx_framerate)
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_ALLOW_NON_DMX_FRAMERATE, 0, 1);

	CLASS_ATTR_CHAR(c, LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, t_lx_service, force_framerate);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_FORCE_FRAMERATE, 0, 1);

	CLASS_ATTR_CHAR(c, LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, t_lx_service, output_empty_universes);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_GLOBAL_CONFIG_OUTPUT_EMPTY_UNIVERSES, 0, 1);


	class_register(CLASS_NOBOX, c);
	lxmax_service_class = c;

	quittask_install((method)lx_onquit, NULL);
	s_lxmax_service_instance = (t_object*)object_new(CLASS_NOBOX, _lxclassnamesym_service);
	if (!s_lxmax_service_instance)
	{
#ifdef NDEBUG
		error("Failed to initialize LXMax V%ld.%ld.%ld - %s", LX_VERSION_MAJOR, LX_VERSION_MINOR, LX_VERSION_BUGFIX);
#else
		error("Failed to initialize LXMax V%ld.%ld.%ld %s - DEBUG",
		            LX_VERSION_MAJOR, LX_VERSION_MINOR, LX_VERSION_BUGFIX,
		            LX_VERSION_SHA);
#endif
	}
}

void lx_onquit(void* a)
{
	if (s_lxmax_service_instance)
		object_free(s_lxmax_service_instance);
}


void* lx_service_new()
{
	t_lx_service* x = (t_lx_service*)object_alloc(lxmax_service_class);
	if (!x)
		return NULL;

	x->is_initial_load_complete = false;

	x->universes = hashtab_new(0);
	x->network_adapters = lx_network_adapter_enumerate();
	x->serial_ports = lx_serial_port_enumerate();

	x->qelem_save = qelem_new(x, (method)lx_service_qelem_save);

	x->io_clock = NULL;
	x->dmx_io_service = NULL;
	x->io_thread = NULL;

	if (object_findregistered(_lxsym_lxmax, _lxsym____lxmax_service))
	{
		object_error((t_object*)x, "Found pre-existing LXMax service instance");
		object_free(x);
		return NULL;
	}

	x->registered_object = (t_object*)object_register(_lxsym_lxmax, _lxsym____lxmax_service, (t_object*)x);
	if (!x->registered_object)
	{
		object_free(x);
		return NULL;
	}
	object_attach_byptr(x, x);

	lx_service_preferences_clear(x);
	lx_service_preferences_load(x);
	lx_service_preferences_save(x);

	x->dmx_io_service = new lxmax::dmx_io_service();

	if (systhread_create((method)lx_service_io_thread, x, 0, 0, 0, &x->io_thread) != MAX_ERR_NONE)
	{
		object_error((t_object*)x, "Failed to start DMX IO thread");
		object_free(x);
		return NULL;
	}

	//x->dmx_io_service->set_universes(x->universes, x->global_artnet_adapter, x->global_sacn_adapter);
	x->io_clock = clock_new(x, (method)lx_service_io_clock_tick);

	x->is_initial_load_complete = true;

#ifdef NDEBUG
	object_post((t_object*)x, "V%ld.%ld.%ld - %s", LX_VERSION_MAJOR, LX_VERSION_MINOR, LX_VERSION_BUGFIX, LX_COPYRIGHT);
#else
	object_post((t_object*)x, "V%ld.%ld.%ld %s - DEBUG - %s",
	            LX_VERSION_MAJOR, LX_VERSION_MINOR, LX_VERSION_BUGFIX,
	            LX_VERSION_SHA, LX_COPYRIGHT);
#endif

	return x;
}

void lx_service_free(t_lx_service* x)
{
	if (x->io_thread)
	{
		if (x->dmx_io_service)
			x->dmx_io_service->stop();

		unsigned ret;
		systhread_join(x->io_thread, &ret);
		x->io_thread = NULL;
	}

	if (x->dmx_io_service)
	{
		delete x->dmx_io_service;
		x->dmx_io_service = NULL;
	}

	if (x->registered_object)
		object_unregister(x->registered_object);

	if (x->qelem_save)
		qelem_free(x->qelem_save);

	if (x->universes)
		object_free(x->universes);

	if (x->network_adapters)
		object_free(x->network_adapters);

	if (x->serial_ports)
		object_free(x->serial_ports);
}


t_max_err lx_service_notify(t_lx_service* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
	if (sender == x)
	{
		if (msg == _lxsym_service_notify_universes_updated || msg == _sym_attr_modified)
		{
			if (x->is_initial_load_complete)
				qelem_set(x->qelem_save);
		}
	}
	else if (object_classname_compare(sender, _lxclassnamesym_universeconfig))
	{
		if (msg == _sym_attr_modified)
		{
			t_symbol* name = ((t_lx_universe_config*)sender)->name;
			t_symbol* attr_name = (t_symbol*)object_method((t_object*)data, _sym_getname);

			if (x->is_initial_load_complete)
				qelem_set(x->qelem_save);
		}
	}

	return MAX_ERR_NONE;
}


void lx_service_qelem_save(t_lx_service* x)
{
	lx_service_preferences_save(x);
	qelem_unset(x->qelem_save);
}


t_max_err lx_service_preferences_load(t_lx_service* x)
{
	short max_pref_path;
	preferences_path(NULL, true, &max_pref_path);

	t_dictionary* dict_preferences;

	if (dictionary_read(LX_PREFERENCES_FILE_NAME, max_pref_path, &dict_preferences) != MAX_ERR_NONE)
		return MAX_ERR_GENERIC;

	t_atomarray* atomarray_universes;
	if (dictionary_getatomarray(dict_preferences, _lxsym_universes, (t_object**)&atomarray_universes) == MAX_ERR_NONE)
	{
		lx_service_universes_clear(x);

		long argc;
		t_atom* argv;
		atomarray_getatoms(atomarray_universes, &argc, &argv);

		for(long i = 0; i < argc; ++i)
		{
			t_dictionary* dict_universe = (t_dictionary*)atom_getobj(argv + i);
			t_lx_universe_config* universe_config = lx_universe_config_new(x, dict_universe);
			lx_service_universes_add(x, universe_config);
		}

		dictionary_deleteentry(dict_preferences, _lxsym_universes);
	}

	attr_dictionary_process(x, dict_preferences);

	freeobject(dict_preferences);

	lx_service_find_global_artnet_interface(x);
	lx_service_find_global_sacn_interface(x);

	return MAX_ERR_NONE;
}

t_max_err lx_service_preferences_save(t_lx_service* x)
{
	t_dictionary* dict_preferences = dictionary_new();
	t_atomarray* atomarray_universes = atomarray_new(0, NULL);

	long argc = 0;
	t_atom* argv = NULL;

	dictionary_appendlong(dict_preferences, _lxsym_lxmax_version_major, LX_VERSION_MAJOR);
	dictionary_appendlong(dict_preferences, _lxsym_lxmax_version_minor, LX_VERSION_MINOR);
	dictionary_appendlong(dict_preferences, _lxsym_lxmax_version_bugfix, LX_VERSION_BUGFIX);

	dictionary_appendsym(dict_preferences, _lxsym_global_config_global_artnet_interface, x->global_artnet_interface);

	object_attr_getvalueof(x, _lxsym_global_config_global_artnet_unicast_addresses, &argc, &argv);
	dictionary_appendatoms(dict_preferences, _lxsym_global_config_global_artnet_unicast_addresses, argc, argv);
	sysmem_freeptr(argv);
	argc = 0;

	dictionary_appendlong(dict_preferences, _lxsym_global_config_artnet_sync_packets_enable, x->artnet_sync_packets_enable);

	dictionary_appendsym(dict_preferences, _lxsym_global_config_global_sacn_interface, x->global_sacn_interface);

	object_attr_getvalueof(x, _lxsym_global_config_global_sacn_unicast_addresses, &argc, &argv);
	dictionary_appendatoms(dict_preferences, _lxsym_global_config_global_sacn_unicast_addresses, argc, argv);
	sysmem_freeptr(argv);
	argc = 0;

	dictionary_appendlong(dict_preferences, _lxsym_global_config_sacn_sync_packets_enable, x->sacn_sync_packets_enable);
	dictionary_appendlong(dict_preferences, _lxsym_global_config_sacn_sync_address, x->sacn_sync_address);

	dictionary_appendfloat(dict_preferences, _lxsym_global_config_max_framerate, x->max_framerate);
	dictionary_appendlong(dict_preferences, _lxsym_global_config_allow_non_dmx_framerate, x->allow_nondmx_framerate);
	dictionary_appendlong(dict_preferences, _lxsym_global_config_force_framerate, x->force_framerate);
	dictionary_appendlong(dict_preferences, _lxsym_global_config_output_empty_universes, x->output_empty_universes);


	long key_count = 0;
	t_symbol** keys = NULL;

	if (hashtab_getkeys(x->universes, &key_count, &keys) == MAX_ERR_NONE)
	{
		for(long i = 0; i < key_count; ++i)
		{
			t_lx_universe_config* config;
			if (hashtab_lookup(x->universes, keys[i], (t_object**)&config) == MAX_ERR_NONE)
			{
				t_dictionary* dict_universe = dictionary_new();
				lx_universe_config_to_dictionary(config, dict_universe);

				t_atom value;
				atom_setobj(&value, dict_universe);
				atomarray_appendatom(atomarray_universes, &value);
			}
		}

		if (keys)
			sysmem_freeptr(keys);
	}

	dictionary_appendatomarray(dict_preferences, _lxsym_universes, (t_object*)atomarray_universes);

	short max_pref_path;
	preferences_path(NULL, true, &max_pref_path);

	const t_max_err result = dictionary_write(dict_preferences, LX_PREFERENCES_FILE_NAME, max_pref_path);

	object_free(dict_preferences);

	return result;
}

void lx_service_preferences_clear(t_lx_service* x)
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
	x->max_framerate = LX_DMX_FRAMERATE_MAX;
	x->allow_nondmx_framerate = false;
	x->force_framerate = false;
	x->output_empty_universes = false;

	lx_service_universes_clear(x);
	lx_service_universes_add(x, lx_universe_config_new(x, NULL));
}


void lx_service_find_global_artnet_interface(t_lx_service* x)
{
	if (x->global_artnet_interface == _lxsym_interface_ip_adapter_all)
	{
		x->global_artnet_adapter = NULL;
		return;
	}
	else
	{
		x->global_artnet_adapter = NULL;
		lx_network_adapter_find(x->network_adapters, x->global_artnet_interface, &x->global_artnet_adapter);
		if (x->global_artnet_adapter)
			x->global_artnet_interface = lx_network_adapter_to_sym(x->global_artnet_adapter);
		else
			x->global_artnet_interface = _lxsym_interface_ip_adapter_all;
	}
}

void lx_service_find_global_sacn_interface(t_lx_service* x)
{
	if (x->global_sacn_interface == _lxsym_interface_ip_adapter_all)
	{
		x->global_sacn_adapter = NULL;
		return;
	}
	else
	{
		x->global_sacn_adapter = NULL;
		lx_network_adapter_find(x->network_adapters, x->global_sacn_interface, &x->global_sacn_adapter);
		if (x->global_sacn_adapter)
			x->global_sacn_interface = lx_network_adapter_to_sym(x->global_sacn_adapter);
		else
			x->global_sacn_interface = _lxsym_interface_ip_adapter_all;
	}
}


void lx_service_io_thread(t_lx_service* x)
{
	if (x->dmx_io_service)
		x->dmx_io_service->run();

	systhread_exit(0);
}

void lx_service_io_clock_tick(t_lx_service* x)
{
	double time_before;
	clock_getftime(&time_before);

	x->dmx_io_service->process_io();

	double time_after;
	clock_getftime(&time_after);

	clock_fdelay(x->io_clock, (1000. / 300.) - (time_after - time_before));
}


t_max_err lx_service_global_artnet_interface_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv)
{
	if (atom_gettype(argv) != A_SYM)
		return MAX_ERR_NONE;

	x->global_artnet_interface = atom_getsym(argv);
	lx_service_find_global_artnet_interface(x);

	return MAX_ERR_NONE;
}

t_max_err lx_service_global_artnet_unicast_addresses_get(t_lx_service* x, t_object* attr, long* argc, t_atom** argv)
{
	char alloc;
	atom_alloc_array(x->global_artnet_unicast_addresses_count, argc, argv, &alloc);

	for(long i = 0; i < *argc; ++i)
		atom_setsym(*argv + i, lx_ip_address_to_sym(x->global_artnet_unicast_addresses + i));

	return MAX_ERR_NONE;
}

t_max_err lx_service_global_artnet_unicast_addresses_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv)
{
	t_atom_long addresses_count = 0;
	t_lx_ip_address* ip_address = x->global_artnet_unicast_addresses;

	for(long i = 0; i < MIN(argc, LX_UNICAST_ADDRESSES_MAX); ++i)
	{
		if (atom_gettype(argv + i) != A_SYM)
			continue;

		if (lx_ip_address_from_sym(atom_getsym(argv + i), ip_address) == MAX_ERR_NONE)
		{
			++ip_address;
			++addresses_count;
		}
	}

	if (addresses_count > 0)
		x->global_artnet_unicast_addresses_count = addresses_count;

	return MAX_ERR_NONE;
}

t_max_err lx_service_global_sacn_interface_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv)
{
	if (atom_gettype(argv) != A_SYM)
		return MAX_ERR_NONE;

	x->global_sacn_interface = atom_getsym(argv);
	lx_service_find_global_sacn_interface(x);

	return MAX_ERR_NONE;
}

t_max_err lx_service_global_sacn_unicast_addresses_get(t_lx_service* x, t_object* attr, long* argc, t_atom** argv)
{
	char alloc;
	atom_alloc_array(x->global_sacn_unicast_addresses_count, argc, argv, &alloc);

	for(long i = 0; i < *argc; ++i)
		atom_setsym(*argv + i, lx_ip_address_to_sym(x->global_sacn_unicast_addresses + i));

	return MAX_ERR_NONE;
}

t_max_err lx_service_global_sacn_unicast_addresses_set(t_lx_service* x, t_object* attr, long argc, t_atom* argv)
{
	t_atom_long addresses_count = 0;
	t_lx_ip_address* ip_address = x->global_sacn_unicast_addresses;

	for(long i = 0; i < MIN(argc, LX_UNICAST_ADDRESSES_MAX); ++i)
	{
		if (atom_gettype(argv + i) != A_SYM)
			continue;

		if (lx_ip_address_from_sym(atom_getsym(argv + i), ip_address) == MAX_ERR_NONE)
		{
			++ip_address;
			++addresses_count;
		}
	}

	if (addresses_count > 0)
		x->global_sacn_unicast_addresses_count = addresses_count;

	return MAX_ERR_NONE;
}