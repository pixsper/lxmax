// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include <ext.h>
#include <ext_obex.h>
#include <commonsyms.h>

#include "lx_version.h"
#include "lx_common_syms.h"

#ifdef liblxmax_EXPORTS
#ifdef _MSC_VER
#define DLL_API __declspec(dllexport)
#elif defined __GNUC__
#define DLL_API __attribute__ ((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define DLL_API __declspec(dllimport)
#elif defined __GNUC__
#define DLL_API __attribute__ ((visibility ("hidden")))
#endif
#endif

BEGIN_USING_C_LINKAGE

#define LX_PREFERENCES_FILE_NAME "lxmaxpreferences.json"

typedef struct _uuid
{
	uint8_t data[16];

} t_lx_uuid;

DLL_API t_lx_common_symbols_table* lx_common_symbols_gettable();

END_USING_C_LINKAGE