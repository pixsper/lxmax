// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_service.h"

#include "lx_universe_config.h"

t_lx_service* lx_service_subscribe(t_object* x)
{
	t_lx_service* service = (t_lx_service*)object_subscribe(_lxsym_lxmax, _lxsym____lxmax_service,
															_lxclassnamesym_service, x);
	
	if (!service)
		object_error((t_object*)x, "Failed to find active LXMax service");

	return service;
}

t_max_err lx_service_unsubscribe(t_object* x, t_lx_service* service)
{
	if (service)
		return object_unsubscribe(_lxsym_lxmax, _lxsym____lxmax_service, _lxclassnamesym_service, x);
	else
		return MAX_ERR_NONE;
}


void lx_service_universes_add(t_lx_service* x, t_lx_universe_config* universe_config)
{
	if (hashtab_store(x->universes, universe_config->name, (t_object*)universe_config) == MAX_ERR_NONE)
	{
		object_attach(_lxsym_lxmax, universe_config->name, x);
		object_notify(x, _lxsym_service_notify_universes_updated, NULL);
	}
}

void lx_service_universes_remove(t_lx_service* x, t_lx_universe_config* universe_config)
{
	object_detach(_lxsym_lxmax, universe_config->name, x);
	if (hashtab_delete(x->universes, universe_config->name) == MAX_ERR_NONE)
		object_notify(x, _lxsym_service_notify_universes_updated, NULL);
}

void lxmax_service_universes_entry_detach(t_hashtab_entry* e, void* x)
{
	t_lx_universe_config* universe_config = (t_lx_universe_config*)e->value;
	object_detach(_lxsym_lxmax, universe_config->name, x);
}

void lx_service_universes_clear(t_lx_service* x)
{
	hashtab_funall(x->universes, (method)lxmax_service_universes_entry_detach, x);
	hashtab_clear(x->universes);
	object_notify(x, _lxsym_service_notify_universes_updated, NULL);
}

void lx_service_universes_quick_patch(t_lx_service* x, long patch_count,
									  e_lx_universe_type type, e_lx_universe_protocol protocol,
									  t_symbol* protocol_mode, t_symbol* protocol_interface,
									  long start_internal_universe, long start_protocol_universe)
{
	for(long i = 0; i < patch_count; ++i)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "Quick Patch Universe %03ld", start_internal_universe + i);

		t_lx_universe_config* config = lx_universe_config_new(x, NULL);
		object_attr_setsym(config, _lxsym_universe_config_label, gensym(buffer));
		object_attr_setlong(config, _lxsym_universe_config_type, type);
		object_attr_setlong(config, _lxsym_universe_config_protocol, protocol);
		object_attr_setsym(config, _lxsym_universe_config_protocol_mode, protocol_mode);
		object_attr_setsym(config, _lxsym_universe_config_protocol_interface, protocol_interface);
		object_attr_setlong(config, _lxsym_universe_config_internal_universe,  start_internal_universe + i);
		object_attr_setlong(config, _lxsym_universe_config_protocol_universe,  start_protocol_universe + i);

		if (hashtab_store(x->universes, config->name, (t_object*)config) == MAX_ERR_NONE)
			object_attach(_lxsym_lxmax, config->name, x);
	}

	if (patch_count > 0)
		object_notify(x, _lxsym_service_notify_universes_updated, NULL);
}