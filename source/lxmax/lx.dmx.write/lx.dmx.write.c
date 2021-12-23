// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h> 
#include <ext_obex.h>
#include <ext_globalsymbol.h>

#include "lxmax.h"

typedef struct t_lx_dmx_write
{
	t_object object;
	t_lx_service* service;

} t_lx_dmx_write;

static t_class* lx_dmx_write_class;

void* lx_dmx_write_new(t_symbol* s, long argc, t_atom* argv);
void lx_dmx_write_free(t_lx_dmx_write* x);
void lx_dmx_write_assist(t_lx_dmx_write* x, void* b, long m, long a, char* s);


void ext_main(void* r)
{
	common_symbols_init();
	lx_sym_init();

	t_class* c;

	c = class_new("lx.dmx.read", (method)lx_dmx_write_new, (method)lx_dmx_write_free, (long)sizeof(t_lx_dmx_write),
		0L, A_GIMME, 0);

	class_addmethod(c, (method)lx_dmx_write_assist, "assist", A_CANT, 0);

	class_register(CLASS_BOX, c);
	lx_dmx_write_class = c;
}




void* lx_dmx_write_new(t_symbol* s, long argc, t_atom* argv)
{
	t_lx_dmx_write* x = (t_lx_dmx_write*)object_alloc(lx_dmx_write_class);
	if (x == NULL)
		return NULL;

	x->service = (t_lx_service*)globalsymbol_reference((t_object*)x, LX_SERVICE_SYMBOL_NAME, LX_SERVICE_CLASS_NAME);
	if (x->service == NULL)
	{
		object_error((t_object*)x, "Failed to find active LXMax service");
		object_free(x);
		return NULL;
	}

	return x;
}

void lx_dmx_write_free(t_lx_dmx_write* x)
{
	globalsymbol_dereference((t_object*)x, LX_SERVICE_SYMBOL_NAME, LX_SERVICE_CLASS_NAME);
}

void lx_dmx_write_assist(t_lx_dmx_write* x, void* b, long m, long a, char* s)
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
