// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h>
#include "lx_common.h"
#include "lx_common_syms.h"


C74_HIDDEN t_lx_common_symbols_table* _lx_common_symbols = NULL;

C74_HIDDEN void lx_common_symbols_init()
{
    _lx_common_symbols = lx_common_symbols_gettable();
}
