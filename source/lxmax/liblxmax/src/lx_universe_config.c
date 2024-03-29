// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_universe_config.h"

#include <ext_globalsymbol.h>

#include "lx_helpers.h"
#include "lx_net.h"
#include "lx_serial.h"
#include "lx_service.h"


t_class* lx_universe_config_class;


t_symbol* lx_universe_type_to_sym(e_lx_universe_type type)
{
	switch (type)
	{
		case LX_UNIVERSE_TYPE_INPUT:
			return _lxsym_type_input;

		case LX_UNIVERSE_TYPE_OUTPUT:
			return _lxsym_type_output;

		case LX_UNIVERSE_TYPE_NONE:
		default:
			return NULL;
	}
}

t_max_err lx_universe_type_from_sym(const t_symbol* s, e_lx_universe_type* type)
{
	if (s == _lxsym_type_input)
	{
		*type = LX_UNIVERSE_TYPE_INPUT;
		return MAX_ERR_NONE;
	}
	else if (s == _lxsym_type_output)
	{
		*type = LX_UNIVERSE_TYPE_OUTPUT;
		return MAX_ERR_NONE;
	}
	else
	{
		return MAX_ERR_GENERIC;
	}
}

void lx_universe_type_get_list(e_lx_universe_protocol protocol, long* argc, t_atom** argv)
{
	char alloc;

	switch (protocol)
	{
		case LX_UNIVERSE_PROTOCOL_ARTNET:
		case LX_UNIVERSE_PROTOCOL_SACN:
		case LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO:
			atom_alloc_array(2, argc, argv, &alloc);
			atom_setsym(*argv, _lxsym_type_output);
			atom_setsym(*argv + 1, _lxsym_type_input);

			break;

		case LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB:
			atom_alloc_array(1, argc, argv, &alloc);
			atom_setsym(*argv, _lxsym_type_output);
			break;

		default:
			break;
	}
}


t_symbol* lx_universe_protocol_to_sym(e_lx_universe_protocol protocol)
{
	switch (protocol)
	{
		case LX_UNIVERSE_PROTOCOL_ARTNET:
			return _lxsym_protocol_artnet;

		case LX_UNIVERSE_PROTOCOL_SACN:
			return _lxsym_protocol_sacn;

	    case LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO:
	        return _lxsym_protocol_enttec_dmxusbpro;

		case LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB:
			return _lxsym_protocol_enttec_opendmxusb;

		case LX_UNIVERSE_PROTOCOL_NONE:
		default:
			return _sym_nothing;
	}
}

t_max_err lx_universe_protocol_from_sym(const t_symbol* s, e_lx_universe_protocol* protocol)
{
	if (s == _lxsym_protocol_artnet)
	{
		*protocol = LX_UNIVERSE_PROTOCOL_ARTNET;
		return MAX_ERR_NONE;
	}
	else if (s == _lxsym_protocol_sacn)
	{
		*protocol = LX_UNIVERSE_PROTOCOL_SACN;
		return MAX_ERR_NONE;
	}
    else if (s == _lxsym_protocol_enttec_dmxusbpro)
    {
        *protocol = LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO;
        return MAX_ERR_NONE;
    }
	else if (s == _lxsym_protocol_enttec_opendmxusb)
	{
		*protocol = LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB;
		return MAX_ERR_NONE;
	}
	else
	{
		return MAX_ERR_GENERIC;
	}
}

void lx_universe_protocol_get_list(long* argc, t_atom** argv)
{
	char alloc;
	atom_alloc_array(4, argc, argv, &alloc);
	atom_setsym(*argv, _lxsym_protocol_artnet);
	atom_setsym(*argv + 1, _lxsym_protocol_sacn);
	atom_setsym(*argv + 2, _lxsym_protocol_enttec_dmxusbpro);
	atom_setsym(*argv + 3, _lxsym_protocol_enttec_opendmxusb);
}

void lx_universe_protocol_mode_get_list(e_lx_universe_type type, e_lx_universe_protocol protocol, long* argc, t_atom** argv)
{
	char alloc;

	switch (protocol)
	{
		case LX_UNIVERSE_PROTOCOL_ARTNET:
		{
			switch (type)
			{
				case LX_UNIVERSE_TYPE_INPUT:
					atom_alloc_array(1, argc, argv, &alloc);
					atom_setsym(*argv, _lxsym_type_input_protocol_artnet_mode_input);
					break;

				case LX_UNIVERSE_TYPE_OUTPUT:
					atom_alloc_array(2, argc, argv, &alloc);
					atom_setsym(*argv, _lxsym_type_output_protocol_artnet_mode_broadcast);
					atom_setsym(*argv + 1, _lxsym_type_output_protocol_artnet_mode_unicast);
					break;

				default:
					break;
			}
		}
			break;

		case LX_UNIVERSE_PROTOCOL_SACN:
		{
			atom_alloc_array(2, argc, argv, &alloc);
			atom_setsym(*argv, _lxsym_type_all_protocol_sacn_mode_multicast);
			atom_setsym(*argv + 1, _lxsym_type_all_protocol_sacn_mode_unicast);
		}
			break;

		case LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO:
		{
			switch (type)
			{
				case LX_UNIVERSE_TYPE_INPUT:
					atom_alloc_array(1, argc, argv, &alloc);
					atom_setsym(*argv, _lxsym_type_input_protocol_dmxusbpro_mode_input);
					break;

				case LX_UNIVERSE_TYPE_OUTPUT:
					atom_alloc_array(1, argc, argv, &alloc);
					atom_setsym(*argv, _lxsym_type_output_protocol_dmxusbpro_mode_output);
					break;

				default:
					break;
			}
		}
			break;

		case LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB:
			atom_alloc_array(1, argc, argv, &alloc);
			atom_setsym(*argv, _lxsym_type_output_protocol_opendmxusb_mode_output);
			break;

		default:
			break;
	}
}

void lx_universe_protocol_interface_get_list(t_lx_service* service, e_lx_universe_protocol protocol, long* argc, t_atom** argv)
{
	switch(protocol)
	{
		case LX_UNIVERSE_PROTOCOL_ARTNET:
		case LX_UNIVERSE_PROTOCOL_SACN:
			lx_network_adapter_get_menu_items(service->network_adapters, argc, argv);
			break;
		case LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO:
		case LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB:
			lx_serial_port_get_menu_items(service->serial_ports, argc, argv);
			break;

		default:
			break;
	}
}


void lx_universe_config_register_class()
{
	t_class* c;

	c = class_new(LX_CLASSNAME_UNIVERSE_CONFIG, (method)lx_universe_config_new, (method)lx_universe_config_free,
	              (long)sizeof(t_lx_universe_config), 0L, A_CANT, 0);

	CLASS_ATTR_SYM(c, LXSYMNAME_UNIVERSE_CONFIG_LABEL, 0, t_lx_universe_config, label);

	CLASS_ATTR_LONG(c, LXSYMNAME_UNIVERSE_CONFIG_TYPE, 0, t_lx_universe_config, type);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_UNIVERSE_CONFIG_TYPE, lx_universe_type_get, lx_universe_type_set);

	CLASS_ATTR_CHAR(c, LXSYMNAME_UNIVERSE_CONFIG_ENABLE, 0, t_lx_universe_config, enable);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_UNIVERSE_CONFIG_ENABLE, 0, 1);

	CLASS_ATTR_LONG(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL, 0, t_lx_universe_config, protocol);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL, lx_universe_protocol_get, lx_universe_protocol_set);

	CLASS_ATTR_SYM(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_MODE, 0, t_lx_universe_config, protocol_mode);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_MODE, NULL, lx_universe_protocol_mode_set);

	CLASS_ATTR_SYM(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_INTERFACE, 0, t_lx_universe_config, protocol_interface);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_INTERFACE, NULL, lx_universe_protocol_interface_set);

	CLASS_ATTR_LONG(c, LXSYMNAME_UNIVERSE_CONFIG_INTERNAL_UNIVERSE, 0, t_lx_universe_config, internal_universe);
	CLASS_ATTR_FILTER_CLIP(c, LXSYMNAME_UNIVERSE_CONFIG_INTERNAL_UNIVERSE, LX_UNIVERSE_MIN, LX_UNIVERSE_MAX);

	CLASS_ATTR_LONG(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_UNIVERSE, 0, t_lx_universe_config, protocol_universe);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_UNIVERSE_CONFIG_PROTOCOL_UNIVERSE, lx_universe_protocol_universe_get, lx_universe_protocol_universe_set);

	CLASS_ATTR_LONG(c, LXSYMNAME_UNIVERSE_CONFIG_PRIORITY, 0, t_lx_universe_config, priority);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_UNIVERSE_CONFIG_PRIORITY, lx_universe_priority_get, lx_universe_priority_set);

	CLASS_ATTR_SYM_VARSIZE(c, LXSYMNAME_UNIVERSE_CONFIG_UNICAST_ADDRESSES, 0, t_lx_universe_config, unicast_addresses,
						   unicast_addresses_count, LX_UNICAST_ADDRESSES_MAX);
	CLASS_ATTR_ACCESSORS(c, LXSYMNAME_UNIVERSE_CONFIG_UNICAST_ADDRESSES, lx_universe_unicast_addresses_get, lx_universe_unicast_addresses_set);

	CLASS_ATTR_SYM(c, LXSYMNAME_UNIVERSE_CONFIG_STATUS, 0, t_lx_universe_config, status);

	class_register(CLASS_NOBOX, c);
	lx_universe_config_class = c;
}

t_lx_universe_config* lx_universe_config_new(t_lx_service* service, t_dictionary* dict)
{
	t_lx_universe_config* x = (t_lx_universe_config*)object_alloc(lx_universe_config_class);
	if (x == NULL)
		return NULL;

	x->service = service;

	x->name = symbol_unique();
	x->registered_object = object_register(_lxsym_lxmax, x->name, x);

	x->adapter = NULL;
    x->serial_port = NULL;

	x->label = gensym("Universe 001");
	x->type = LX_UNIVERSE_TYPE_OUTPUT;
	x->enable = true;
	x->protocol = LX_UNIVERSE_PROTOCOL_ARTNET;
	x->protocol_mode = _lxsym_type_output_protocol_artnet_mode_broadcast;
	x->protocol_interface = _lxsym_interface_ip_adapter_global;
	x->internal_universe = 1;
	x->protocol_universe = 1;
	x->priority = 100;
	memset(x->unicast_addresses, 0, LX_UNICAST_ADDRESSES_MAX);
	x->unicast_addresses_count = 0;

	x->status = _sym_nothing;

	if (dict != NULL)
		attr_dictionary_process(x, dict);

	return x;
}

DLL_API t_lx_universe_config* lx_universe_config_clone(t_lx_service* service, t_lx_universe_config* source)
{
	t_lx_universe_config* x = lx_universe_config_new(service, NULL);

	if (source)
	{
		x->label = source->label;
		x->type = source->type;
		x->enable =source->enable;
		x->protocol = source->protocol;
		x->protocol_mode = source->protocol_mode;
		x->protocol_interface = source->protocol_interface;
		x->internal_universe = source->internal_universe;
		x->protocol_universe = source->protocol_universe;
		x->priority = source->priority;
		memcpy(x->unicast_addresses, source->unicast_addresses, LX_UNICAST_ADDRESSES_MAX * sizeof(t_lx_ip_address));
		x->unicast_addresses_count = source->unicast_addresses_count;
	}

	lx_universe_config_find_interface(x);

	return x;
}

void lx_universe_config_free(t_lx_universe_config* x)
{
	object_unregister(x->registered_object);
	object_free(x->unicast_addresses);
}

void lx_universe_config_increment(t_lx_universe_config* x, long count)
{
	t_symbol* label;
	if (lx_helpers_symbol_increment(x->label, count, &label) == MAX_ERR_NONE)
		object_attr_setsym(x, _lxsym_universe_config_label, label);

	object_attr_setlong(x, _lxsym_universe_config_internal_universe, x->internal_universe + count);
	object_attr_setlong(x, _lxsym_universe_config_protocol_universe, x->protocol_universe + count);
}

void  lx_universe_config_find_interface(t_lx_universe_config* x)
{
    switch (x->protocol)
    {
        case LX_UNIVERSE_PROTOCOL_ARTNET:
        case LX_UNIVERSE_PROTOCOL_SACN:
        {
            if ((x->protocol != LX_UNIVERSE_PROTOCOL_ARTNET && x->protocol != LX_UNIVERSE_PROTOCOL_SACN)
                || x->protocol_interface == _lxsym_interface_ip_adapter_all)
            {
                x->adapter = NULL;
                x->serial_port = NULL;
            }
            else if (x->protocol_interface == _lxsym_interface_ip_adapter_global
				|| x->protocol_interface == _lxsym_interface_none)
            {
                x->serial_port = NULL;

				x->protocol_interface = _lxsym_interface_ip_adapter_global;
                switch (x->protocol)
                {
                    case LX_UNIVERSE_PROTOCOL_ARTNET:
                        x->adapter = x->service->global_artnet_adapter;
                        break;
                    case LX_UNIVERSE_PROTOCOL_SACN:
                        x->adapter = x->service->global_sacn_adapter;
                        break;
                    default:
                        x->adapter = NULL;
                        break;
                }
            }
            else
            {
                x->serial_port = NULL;
                x->adapter = NULL;
                if (lx_network_adapter_find(x->service->network_adapters, x->protocol_interface, &x->adapter) == MAX_ERR_NONE)
                {
					if (x->adapter)
						x->protocol_interface = lx_network_adapter_to_sym(x->adapter);
					else
						x->protocol_interface = _lxsym_interface_ip_adapter_all;
                }
				else
				{
					x->protocol_interface = _lxsym_interface_ip_adapter_global;
					switch (x->protocol)
					{
						case LX_UNIVERSE_PROTOCOL_ARTNET:
							x->adapter = x->service->global_artnet_adapter;
							break;
						case LX_UNIVERSE_PROTOCOL_SACN:
							x->adapter = x->service->global_sacn_adapter;
							break;
						default:
							x->adapter = NULL;
							break;
					}
				}
                
            }
        }
            break;

        case LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO:
		case LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB:
        {
            x->adapter = NULL;
            lx_serial_port_find(x->service->serial_ports, x->protocol_interface, &x->serial_port);
            if (x->serial_port)
                x->protocol_interface = lx_serial_port_to_sym(x->serial_port);
            else
                x->protocol_interface = _lxsym_interface_none;
        }
            break;

		default:
			break;
    }
}


void lx_universe_config_to_dictionary(const t_lx_universe_config* x, t_dictionary* dict)
{
	dictionary_appendsym(dict, _lxsym_universe_config_label, x->label);
	dictionary_appendsym(dict, _lxsym_universe_config_type, lx_universe_type_to_sym(x->type));
	dictionary_appendlong(dict, _lxsym_universe_config_enable, x->enable);
	dictionary_appendsym(dict, _lxsym_universe_config_protocol, lx_universe_protocol_to_sym(x->protocol));
	dictionary_appendsym(dict, _lxsym_universe_config_protocol_mode, x->protocol_mode);
	dictionary_appendsym(dict, _lxsym_universe_config_protocol_interface, x->protocol_interface);
	dictionary_appendlong(dict, _lxsym_universe_config_internal_universe, x->internal_universe);
	dictionary_appendlong(dict, _lxsym_universe_config_protocol_universe, x->protocol_universe);
	dictionary_appendlong(dict, _lxsym_universe_config_priority, x->priority);

	char alloc;
	long argc = 0;
	t_atom* argv = NULL;
	atom_alloc_array(x->unicast_addresses_count, &argc, &argv, &alloc);

	for(long i = 0; i < argc; ++i)
		atom_setsym(argv + i, lx_ip_address_to_sym(x->unicast_addresses + i));

	dictionary_appendatoms(dict, _lxsym_universe_config_unicast_addresses, argc, argv);

	sysmem_freeptr(argv);
}



t_max_err lx_universe_type_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv)
{
	char alloc;
	atom_alloc(argc, argv, &alloc);
	atom_setsym(*argv, lx_universe_type_to_sym(x->type));
	return MAX_ERR_NONE;
}

t_max_err lx_universe_type_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	switch(atom_gettype(argv))
	{
		case A_SYM:
			lx_universe_type_from_sym(atom_getsym(argv), &x->type);
			break;

		case A_LONG:
		case A_FLOAT:
		{
			t_atom_long value = atom_getlong(argv);
			if (value > LX_UNIVERSE_TYPE_NONE && value < LX_UNIVERSE_TYPE_COUNT)
				x->type = (e_lx_universe_type)value;
		}
			break;

		default:
			break;
	}

    object_attr_setsym(x, _lxsym_universe_config_protocol_mode, x->protocol_mode);

    return MAX_ERR_NONE;
}

t_max_err lx_universe_protocol_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv)
{
	char alloc;
	atom_alloc(argc, argv, &alloc);
	atom_setsym(*argv, lx_universe_protocol_to_sym(x->protocol));
	return MAX_ERR_NONE;
}

t_max_err lx_universe_protocol_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	switch (atom_gettype(argv))
	{
		case A_SYM:
			lx_universe_protocol_from_sym(atom_getsym(argv), &x->protocol);
			break;

		case A_LONG:
		case A_FLOAT:
		{
			t_atom_long value = atom_getlong(argv);
			if (value > LX_UNIVERSE_PROTOCOL_NONE && value < LX_UNIVERSE_PROTOCOL_COUNT)
				x->protocol = (e_lx_universe_protocol)value;
		}
			break;

		default:
			break;
	}

	object_attr_setsym(x, _lxsym_universe_config_protocol_mode, x->protocol_mode);
    lx_universe_config_find_interface(x);

	return MAX_ERR_NONE;
}

t_max_err lx_universe_protocol_mode_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	if (atom_gettype(argv) != A_SYM)
		return MAX_ERR_NONE;

	x->protocol_mode = atom_getsym(argv);

	switch (x->protocol)
	{
		case LX_UNIVERSE_PROTOCOL_ARTNET:

			switch (x->type)
			{
				case LX_UNIVERSE_TYPE_INPUT:
					if (x->protocol_mode != _lxsym_type_input_protocol_artnet_mode_input)
						x->protocol_mode = _lxsym_type_input_protocol_artnet_mode_input;
					break;

				case LX_UNIVERSE_TYPE_OUTPUT:
					if (x->protocol_mode != _lxsym_type_output_protocol_artnet_mode_broadcast
						&& x->protocol_mode != _lxsym_type_output_protocol_artnet_mode_unicast)
					{
						x->protocol_mode = _lxsym_type_output_protocol_artnet_mode_broadcast;
					}
					break;

				default:
					break;
			}

			x->protocol_universe = CLAMP(x->protocol_universe, LX_UNIVERSE_ARTNET_MIN, LX_UNIVERSE_ARTNET_MAX);
			break;

		case LX_UNIVERSE_PROTOCOL_SACN:

			if (x->protocol_mode != _lxsym_type_all_protocol_sacn_mode_multicast
				&& x->protocol_mode != _lxsym_type_all_protocol_sacn_mode_unicast)
			{
				x->protocol_mode = _lxsym_type_all_protocol_sacn_mode_multicast;
			}

			x->protocol_universe = CLAMP(x->protocol_universe, LX_UNIVERSE_SACN_MIN, LX_UNIVERSE_SACN_MAX);
			break;

        case LX_UNIVERSE_PROTOCOL_ENTTEC_DMXUSBPRO:

            switch (x->type)
            {
                case LX_UNIVERSE_TYPE_INPUT:
                    x->protocol_mode = _lxsym_type_input_protocol_dmxusbpro_mode_input;
                    break;

                case LX_UNIVERSE_TYPE_OUTPUT:
                    x->protocol_mode = _lxsym_type_output_protocol_dmxusbpro_mode_output;
                    break;

				default:
					break;
            }

            x->protocol_universe = CLAMP(x->protocol_universe, LX_UNIVERSE_DMXUSBPRO_MIN, LX_UNIVERSE_DMXUSBPRO_MAX);
            break;

		case LX_UNIVERSE_PROTOCOL_ENTTEC_OPENDMXUSB:
			x->type = LX_UNIVERSE_TYPE_OUTPUT;
			x->protocol_mode = _lxsym_type_output_protocol_dmxusbpro_mode_output;
			x->protocol_universe = CLAMP(x->protocol_universe, LX_UNIVERSE_OPENDMXUSB_MIN, LX_UNIVERSE_OPENDMXUSB_MAX);

		default:
			break;
	}

	return MAX_ERR_NONE;
}

t_max_err lx_universe_protocol_interface_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	if (atom_gettype(argv) != A_SYM)
		return MAX_ERR_NONE;
	
	x->protocol_interface = atom_getsym(argv);
    lx_universe_config_find_interface(x);

	return MAX_ERR_NONE;
}

t_max_err lx_universe_protocol_universe_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv)
{
	char alloc;
	atom_alloc(argc, argv, &alloc);

	if (x->protocol == LX_UNIVERSE_PROTOCOL_ARTNET)
		atom_setsym(*argv, lx_dmx_universe_to_artnet_format(x->protocol_universe));
	else
		atom_setlong(*argv, x->protocol_universe);

	return MAX_ERR_NONE;
}

t_max_err lx_universe_protocol_universe_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	t_atom_long value;

	switch (atom_gettype(argv))
	{
		case A_SYM:
			if (lx_dmx_universe_from_artnet_format(atom_getsym(argv), &value) != MAX_ERR_NONE)
				return MAX_ERR_NONE;
			break;

		case A_LONG:
		case A_FLOAT:
			value = atom_getlong(argv);
			break;

		default:
			return MAX_ERR_NONE;
	}

	switch (x->protocol)
	{
		case LX_UNIVERSE_PROTOCOL_ARTNET:
			value = CLAMP(value, LX_UNIVERSE_ARTNET_MIN, LX_UNIVERSE_ARTNET_MAX);
			break;
		case LX_UNIVERSE_PROTOCOL_SACN:
			value = CLAMP(value, LX_UNIVERSE_SACN_MIN, LX_UNIVERSE_SACN_MAX);
			break;

		default:
			break;
	}

	x->protocol_universe = value;

	return MAX_ERR_NONE;
}

t_max_err lx_universe_priority_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv)
{
	char alloc;
	atom_alloc(argc, argv, &alloc);

	if (x->protocol == LX_UNIVERSE_PROTOCOL_SACN)
		atom_setlong(*argv, x->priority);
	else
		atom_setsym(*argv, _lxsym_not_applicable);

	return MAX_ERR_NONE;
}

t_max_err lx_universe_priority_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	if (x->protocol != LX_UNIVERSE_PROTOCOL_SACN)
		return MAX_ERR_NONE;

	x->priority = CLAMP(atom_getlong(argv), LX_PRIORITY_SACN_MIN, LX_PRIORITY_SACN_MAX);

	return MAX_ERR_NONE;
}

t_max_err lx_universe_unicast_addresses_get(t_lx_universe_config* x, t_object* attr, long* argc, t_atom** argv)
{
	if (x->protocol_mode == _lxsym_type_output_protocol_artnet_mode_unicast
		|| x->protocol_mode== _lxsym_type_all_protocol_sacn_mode_unicast)
	{
		char alloc;
		atom_alloc_array(x->unicast_addresses_count, argc, argv, &alloc);

		for(long i = 0; i < *argc; ++i)
			atom_setsym(*argv + i, lx_ip_address_to_sym(x->unicast_addresses + i));
	}
	else if (x->protocol_mode == _lxsym_type_output_protocol_artnet_mode_broadcast)
	{
		if (x->protocol_interface == _lxsym_interface_ip_adapter_all)
		{
			char alloc;
			atom_alloc(argc, argv, &alloc);
			atom_setsym(*argv, _lxsym_ip_address_broadcast);
		}
		else if (x->adapter)
		{
			t_linklist* broadcast_addresses = lx_network_adapter_get_broadcast_ips(x->adapter);

			char alloc;
			atom_alloc_array(linklist_getsize(broadcast_addresses), argc, argv, &alloc);
			t_atom* av = *argv;
			
			t_lx_ip_address* address = (t_lx_ip_address*)linklist_getindex(broadcast_addresses, 0);
			while(address)
			{
				atom_setsym(av++, lx_ip_address_to_sym(address));
				linklist_next(broadcast_addresses, address, (void**)&address);
			}
		}
	}
	else if (x->protocol_mode == _lxsym_type_all_protocol_sacn_mode_multicast)
	{
		char alloc;
		atom_alloc(argc, argv, &alloc);

		t_lx_ip_address multicast_ip;
		lx_dmx_universe_to_sacn_multicast_ip(x->protocol_universe, &multicast_ip);
		atom_setsym(*argv, lx_ip_address_to_sym(&multicast_ip));
	}
	else
	{
		char alloc;
		atom_alloc(argc, argv, &alloc);
		atom_setsym(*argv, _lxsym_not_applicable);
	}

	return MAX_ERR_NONE;
}

t_max_err lx_universe_unicast_addresses_set(t_lx_universe_config* x, t_object* attr, long argc, t_atom* argv)
{
	if (x->type == LX_UNIVERSE_TYPE_INPUT || 
		(x->protocol_mode != _lxsym_type_output_protocol_artnet_mode_unicast
		&& x->protocol_mode== _lxsym_type_all_protocol_sacn_mode_unicast))
	{
		return MAX_ERR_NONE;
	}

	if (argc == 0)
	{
		x->unicast_addresses_count = 0;
	}
	else
	{
		t_atom_long addresses_count = 0;
		t_lx_ip_address* ip_address = x->unicast_addresses;

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
			x->unicast_addresses_count = addresses_count;
	}

	return MAX_ERR_NONE;
}
