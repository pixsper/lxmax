// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "lx_common.h"

BEGIN_USING_C_LINKAGE

typedef struct _lx_serial_port
{
	t_symbol* description;
	t_symbol* hardware_id;
	t_symbol* port;

} t_lx_serial_port;

DLL_API t_hashtab* lx_serial_port_enumerate();

DLL_API void lx_serial_port_get_menu_items(t_hashtab* ports, long* argc, t_atom** argv);
DLL_API t_max_err lx_serial_port_find(t_hashtab* ports, t_symbol* sym, t_lx_serial_port** port);

DLL_API t_symbol* lx_serial_port_to_sym(const t_lx_serial_port* port);

END_USING_C_LINKAGE