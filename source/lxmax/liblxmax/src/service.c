// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "service.h"

#include "universe_config.h"

void lxmax_service_universes_add(t_lx_service* x, t_lx_universe_config* universe_config, long index)
{
	if (index == -1)
		linklist_append(x->universes, universe_config);
	else
		linklist_insertindex(x->universes, universe_config, index);

	object_notify(x, _lxsym_service_notify_universes_updated, NULL);
}

void lxmax_service_universes_remove(t_lx_service* x, t_lx_universe_config* universe_config)
{
	linklist_deleteobject(x->universes, universe_config);
	object_notify(x, _lxsym_service_notify_universes_updated, NULL);
}

void lxmax_service_universes_clear(t_lx_service* x)
{
	linklist_clear(x->universes);
	object_notify(x, _lxsym_service_notify_universes_updated, NULL);
}