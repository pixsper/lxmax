// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_helpers.h"

#include <string>
#include <sstream>
#include <regex>

t_max_err lx_helpers_symbol_increment(t_symbol* src, long count, t_symbol** output)
{
	const std::string source(src->s_name);
	size_t last_index = source.find_last_not_of("0123456789");
	if (last_index == source.length() - 1)
		return MAX_ERR_GENERIC;

	std::string value_string = source.substr(last_index + 1);

	const long value = std::stol(value_string) + count;
	std::string output_value_string = std::to_string(value);
	const long zero_padding = value_string.length() - output_value_string.length();

	std::ostringstream ss;
	ss << source.substr(0, last_index + 1);
	for(long i = 0; i < zero_padding; ++i)
		ss << '0';

	ss << output_value_string;

	*output = gensym(ss.str().c_str());

	return MAX_ERR_NONE;
}

t_symbol* lx_helpers_symbol_join(long src_count, t_symbol** src, const char* separator)
{
	std::stringstream ss;

	bool is_first = true;

	for(long i = 0; i < src_count; ++i)
	{
		if (is_first)
			is_first = false;
		else
			ss << separator;

		ss << src[i]->s_name;
	}

	return gensym(ss.str().c_str());
}

t_max_err lx_helpers_regex_matches(t_symbol* src, const char* regex, long* matches_count, t_symbol*** matches)
{
	const std::regex regex_expression(regex);

	std::cmatch regex_matches;
	if (!std::regex_match(src->s_name, regex_matches, regex_expression))
		return MAX_ERR_GENERIC;

	*matches_count = regex_matches.size();
	*matches = (t_symbol**)sysmem_newptr(sizeof(t_symbol*) * regex_matches.size());

	t_symbol** sym_match = *matches;

	for(long i = 0; i < regex_matches.size(); ++i)
		*sym_match++ = gensym(regex_matches[i].str().c_str());

	return MAX_ERR_NONE;
}


long lx_hashtab_get_sorted_compare(void *a, void *b)
{
	t_atomarray* a1 = (t_atomarray*)a;
	t_atomarray* a2 = (t_atomarray*)b;

	if (atomarray_getsize(a1) < atomarray_getsize(a2))
		return true;

	if (atomarray_getsize(a1) > atomarray_getsize(a2))
		return false;

	if (atomarray_getsize(a1) < 2)
		return false;

	t_atom argv1;
	atomarray_getindex(a1, 1, &argv1);
	t_atom argv2;
	atomarray_getindex(a2, 1, &argv2);

	long atype1 = atom_gettype(&argv1);
	long atype2 = atom_gettype(&argv2);

	if (atype1 != atype2)
		return false;

	switch (atype1)
	{
		case A_LONG:
			return atom_getlong(&argv1) < atom_getlong(&argv2);

		case A_FLOAT:
			return atom_getfloat(&argv1) < atom_getfloat(&argv2);

		case A_SYM:
			return strcmp(atom_getsym(&argv1)->s_name, atom_getsym(&argv2)->s_name) < 0;

		default:
			return false;
	}
}

t_linklist* lx_helpers_hashtab_get_sorted_by_attribute(t_hashtab* hashtab, t_symbol* sort_attr_name)
{
	t_linklist* sorted = linklist_new();

	long key_count;
	t_symbol** keys = NULL;

	if (hashtab_getkeys(hashtab, &key_count, &keys) == MAX_ERR_NONE)
	{
		for (long i = 0; i < key_count; ++i)
		{
			t_hashtab_entry* entry;
			if (hashtab_lookupentry(hashtab, keys[i], &entry) == MAX_ERR_NONE)
			{
				long argc = 0;
				t_atom* argv = NULL;

				if (object_attr_getvalueof(entry->value, sort_attr_name, &argc, &argv) == MAX_ERR_NONE
				    && argc > 0)
				{
					t_atomarray* pair = atomarray_new(0, NULL);

					t_atom key_atom;
					atom_setsym(&key_atom, keys[i]);

					atomarray_appendatom(pair, &key_atom);
					atomarray_appendatom(pair, argv);

					linklist_append(sorted, pair);

					if (argv)
						sysmem_freeptr(argv);
				}
			}
		}

		if (keys)
			sysmem_freeptr(keys);
	}

	linklist_sort(sorted, lx_hashtab_get_sorted_compare);

	return sorted;
}

t_symbol* lx_helpers_hashtab_get_first(t_hashtab* hashtab, t_symbol* sort_attr_name)
{
	t_linklist* sorted = lx_helpers_hashtab_get_sorted_by_attribute(hashtab, sort_attr_name);

	if (linklist_getsize(sorted) == 0)
		return NULL;

	t_atomarray* pair = (t_atomarray*)linklist_getindex(sorted, 0);
	t_atom name_argv;
	atomarray_getindex(pair, 0, &name_argv);

	freeobject(sorted);

	return atom_getsym(&name_argv);
}

t_symbol* lx_helpers_hashtab_get_last(t_hashtab* hashtab, t_symbol* sort_attr_name)
{
	t_linklist* sorted = lx_helpers_hashtab_get_sorted_by_attribute(hashtab, sort_attr_name);

	long list_length = linklist_getsize(sorted);

	if (list_length == 0)
		return NULL;

	t_atomarray* pair = (t_atomarray*)linklist_getindex(sorted, list_length - 1);
	t_atom name_argv;
	atomarray_getindex(pair, 0, &name_argv);

	freeobject(sorted);

	return atom_getsym(&name_argv);
}

t_symbol* lx_helpers_hashtab_get_next(t_hashtab* hashtab, t_symbol* key, t_symbol* sort_attr_name)
{
	t_linklist* sorted = lx_helpers_hashtab_get_sorted_by_attribute(hashtab, sort_attr_name);

	t_symbol* result = NULL;

	t_atomarray* pair = (t_atomarray*)linklist_getindex(sorted, 0);
	while(pair)
	{
		t_atom name_argv;
		atomarray_getindex(pair, 0, &name_argv);
		if (atom_getsym(&name_argv) == key)
		{
			linklist_next(sorted, (void*)pair, (void**)&pair);

			if (pair)
			{
				atomarray_getindex(pair, 0, &name_argv);
				result = atom_getsym(&name_argv);
			}

			break;
		}

		linklist_next(sorted, (void*)pair, (void**)&pair);
	}

	freeobject(sorted);

	return result;
}

t_symbol* lx_helpers_hashtab_get_prev(t_hashtab* hashtab, t_symbol* key, t_symbol* sort_attr_name)
{
	t_linklist* sorted = lx_helpers_hashtab_get_sorted_by_attribute(hashtab, sort_attr_name);

	t_symbol* result = NULL;

	t_atomarray* pair = (t_atomarray*)linklist_getindex(sorted, 0);
	while(pair)
	{
		t_atom name_argv;
		atomarray_getindex(pair, 0, &name_argv);
		if (atom_getsym(&name_argv) == key)
		{
			linklist_prev(sorted, (void*)pair, (void**)&pair);

			if (pair)
			{
				atomarray_getindex(pair, 0, &name_argv);
				result = atom_getsym(&name_argv);
			}

			break;
		}

		linklist_next(sorted, (void*)pair, (void**)&pair);
	}

	freeobject(sorted);

	return result;
}