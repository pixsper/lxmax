// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_serial.h"

#include "lx_helpers.h"

void lx_serial_port_get_menu_items(t_hashtab* ports, long* argc, t_atom** argv)
{
    char alloc;

    long key_count = 0;
    t_symbol** keys = NULL;
    if (hashtab_getkeys(ports, &key_count, &keys) == MAX_ERR_NONE)
    {
	    atom_alloc_array(key_count + 1, argc, argv, &alloc);
	    atom_setsym(*argv, _lxsym_interface_none);

	    for (long i = 0; i < key_count; ++i)
	    {
			t_symbol* key = *(keys + i);
			t_lx_serial_port* port = NULL;

			if (hashtab_lookup(ports, key, (t_object**)&port) == MAX_ERR_NONE)
				atom_setsym(*argv + i + 1, lx_serial_port_to_sym(port));
	    }

		if (keys)
			sysmem_freeptr(keys);
	}
}

t_max_err lx_serial_port_find(t_hashtab* ports, t_symbol* sym, t_lx_serial_port** port)
{
	t_max_err result = MAX_ERR_NONE;

	const char* adapter_sym_regex = "(.*) \\((.*)\\)";

	long matches_count = 0;
	t_symbol** matches = NULL;
	if (lx_helpers_regex_matches(sym, adapter_sym_regex, &matches_count, &matches) == MAX_ERR_NONE)
    {
	    t_symbol* port_description = matches[1];
        t_symbol* port_name = matches[2];

        long key_count = 0;
        t_symbol** keys = NULL;
        if (hashtab_getkeys(ports, &key_count, &keys) == MAX_ERR_NONE)
        {
	        for (long i = 0; i < key_count; ++i)
	        {
		        t_symbol* key = *(keys + i);
		        t_lx_serial_port* p = NULL;

		        if (hashtab_lookup(ports, key, (t_object**)&p) == MAX_ERR_NONE)
		        {
			        if (p->port == port_name)
			        {
						*port = p;
						break;
					}
		        }
	        }

	        if (!result && port_description != _lxsym_interface_none)
	        {
		        for (long i = 0; i < key_count; ++i)
		        {
			        t_symbol* key = *(keys + i);
			        t_lx_serial_port* p = NULL;

			        if (hashtab_lookup(ports, key, (t_object **) &p) == MAX_ERR_NONE)
			        {
				        if (p->description == port_description)
				        {
							*port = p;
							break;
						}
			        }
		        }
	        }

			if (keys)
				sysmem_freeptr(keys);
		}

	    if (matches)
		    sysmem_freeptr(matches);
    }
	else
	{
		result = MAX_ERR_GENERIC;
	}

    return result;
}

t_symbol* lx_serial_port_to_sym(const t_lx_serial_port* port)
{
#ifdef WIN_VERSION
    return port->port;
#else
    const size_t buffer_length = 256;
    char buffer[buffer_length];

    t_symbol* description = port->description;

    if (description == _lxsym_not_applicable)
        description = _lxsym_unknown;

    snprintf(buffer, buffer_length, "%s (%s)", description->s_name, port->port->s_name);
    return gensym(buffer);
#endif
}