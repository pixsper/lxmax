// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#pragma once

#include "lx_common.h"

BEGIN_USING_C_LINKAGE

DLL_API t_max_err lx_helpers_symbol_increment(t_symbol* src, long count, t_symbol** output);
DLL_API t_symbol* lx_helpers_symbol_join(long src_count, t_symbol** src, const char* separator);

DLL_API t_max_err lx_helpers_regex_matches(t_symbol* src, const char* regex, long* matches_count, t_symbol*** matches);

DLL_API t_linklist* lx_helpers_hashtab_get_sorted_by_attribute(t_hashtab* hashtab, t_symbol* sort_attr_name);
DLL_API t_symbol* lx_helpers_hashtab_get_first(t_hashtab* hashtab, t_symbol* sort_attr_name);
DLL_API t_symbol* lx_helpers_hashtab_get_last(t_hashtab* hashtab, t_symbol* sort_attr_name);
DLL_API t_symbol* lx_helpers_hashtab_get_next(t_hashtab* hashtab, t_symbol* key, t_symbol* sort_attr_name);
DLL_API t_symbol* lx_helpers_hashtab_get_prev(t_hashtab* hashtab, t_symbol* key, t_symbol* sort_attr_name);


END_USING_C_LINKAGE