// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h> 
#include <ext_obex.h>

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
	lx_common_symbols_init();

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

	x->service = lx_service_subscribe((t_object*)x);
	if (!x->service)
	{
		object_free(x);
		return NULL;
	}

	return x;
}

void lx_dmx_write_free(t_lx_dmx_write* x)
{
	lx_service_unsubscribe((t_object*)x, x->service);
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
