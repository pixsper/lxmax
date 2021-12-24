// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h>
#include <ext_obex.h>
#include <ext_dictobj.h>
#include <commonsyms.h>
#include <jdataview.h>

#include "lxmax.h"

typedef enum _column_kind
{
	LX_CONFIG_UI_COLUMN_TEXT,
	LX_CONFIG_UI_COLUMN_TEXT_LIST,
	LX_CONFIG_UI_COLUMN_TOGGLE,
	LX_CONFIG_UI_COLUMN_MENU,
	LX_CONFIG_UI_COLUMN_INTEGER,
	LX_CONFIG_UI_COLUMN_STATIC

} e_lx_config_ui_column_kind;

typedef struct _lx_config_ui
{
	t_jbox box;
	t_lx_service* service;

	t_object* dataview;
	t_object* patcherview;

	t_symbol* selected_row;
	t_bool is_disable_external_refresh;

} t_lx_config_ui;

static t_class* lx_config_ui_class;

void* lx_config_ui_new(t_symbol* s, short argc, t_atom* argv);
void lx_config_ui_free(t_lx_config_ui* x);

void lx_config_ui_new_patcherview(t_lx_config_ui* x, t_object* patcherview);
void lx_config_ui_free_patcherview(t_lx_config_ui* x, t_object* patcherview);

t_max_err lx_config_ui_notify(t_lx_config_ui* x, t_symbol* s, t_symbol* msg, void* sender, void* data);

void lx_config_ui_add_universe(t_lx_config_ui* x);
void lx_config_ui_remove_universe(t_lx_config_ui* x);

void lx_config_ui_get_cell_text(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, char* text, long max_length);
void lx_config_ui_get_cell_value(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long* argc, t_atom* argv);
void lx_config_ui_get_cell_menu(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long* argc, t_atom* argv,
								char** enabled, long* current_item_index);
void lx_config_ui_set_value(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long argc, t_atom* argv);
void lx_config_ui_selected_row(t_lx_config_ui* x, t_rowref* rr);

void lx_config_ui_toggle_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_menu_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_text_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_text_list_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_integer_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);

void lx_config_ui_add_column(t_lx_config_ui* x, t_symbol* name, t_symbol* label, long width,
							 e_lx_config_ui_column_kind kind, t_bool is_numeric);
void lx_config_ui_refresh_data(t_lx_config_ui* x);


void ext_main(void* r)
{
	common_symbols_init();
    lx_common_symbols_init();

	t_class* c = class_new("lx.config.ui", (method)lx_config_ui_new, (method)lx_config_ui_free, sizeof(t_lx_config_ui), 
		(method)NULL,A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_COLOR);

	class_addmethod(c, (method)lx_config_ui_new_patcherview, "newpatcherview", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_free_patcherview, "freepatcherview", A_CANT, 0);

	class_addmethod(c, (method)lx_config_ui_notify, "notify", A_CANT, 0);

	class_addmethod(c, (method)lx_config_ui_add_universe, "add_universe", 0);
	class_addmethod(c, (method)lx_config_ui_remove_universe, "remove_universe", 0);

	class_addmethod(c, (method)lx_config_ui_get_cell_text, "getcelltext", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_get_cell_value, "getcellvalue", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_get_cell_menu, "getcellmenu", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_set_value, "setvalue", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_selected_row, "selectedrow", A_CANT, 0);

	class_addmethod(c, (method)lx_config_ui_toggle_component, "toggle_component", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_menu_component, "menu_component", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_text_component, "text_component", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_text_list_component, "text_list_component", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_integer_component, "integer_component", A_CANT, 0);
	

	class_register(CLASS_BOX, c);
	lx_config_ui_class = c;
}



void* lx_config_ui_new(t_symbol* s, short argc, t_atom* argv)
{
	t_dictionary* d = object_dictionaryarg(argc, argv);
	if (d == NULL)
		return NULL;

	t_lx_config_ui* x = (t_lx_config_ui*)object_alloc(lx_config_ui_class);
	if (x == NULL)
		return NULL;

	const long flags = 0
			| JBOX_DRAWFIRSTIN
			| JBOX_NODRAWBOX
			| JBOX_DRAWINLAST
			| JBOX_GROWBOTH
			| JBOX_HILITE;

	jbox_new(&x->box, flags, argc, argv);
	x->box.b_firstin = (t_object*)x;


	x->dataview = (t_object*)jdataview_new();
	jdataview_setclient(x->dataview, (t_object*)x);
	jdataview_setcolumnheaderheight(x->dataview, 40);
	jdataview_setheight(x->dataview, 16);
	x->patcherview = NULL;

	x->service = lx_service_subscribe((t_object*)x);
	if (!x->service)
	{
		object_free(x);
		return NULL;
	}

	lx_config_ui_add_column(x, _lxsym_universe_config_label, gensym("Label"),
	                        200, LX_CONFIG_UI_COLUMN_TEXT, false);
	lx_config_ui_add_column(x, _lxsym_universe_config_type, gensym("Type"),
	                        70, LX_CONFIG_UI_COLUMN_MENU, false);
	lx_config_ui_add_column(x, _lxsym_universe_config_enable, gensym("On"),
	                        40, LX_CONFIG_UI_COLUMN_TOGGLE, true);
	lx_config_ui_add_column(x, _lxsym_universe_config_status, gensym("Status"),
	                        60, LX_CONFIG_UI_COLUMN_STATIC, true);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol, gensym("Protocol / Device"),
	                        100, LX_CONFIG_UI_COLUMN_MENU, false);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol_mode, gensym("Mode"),
	                        100, LX_CONFIG_UI_COLUMN_MENU, false);
	lx_config_ui_add_column(x, _lxsym_universe_config_internal_universe, gensym("Internal Universe"),
	                        100, LX_CONFIG_UI_COLUMN_INTEGER, true);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol_universe, gensym("Protocol Universe"),
	                        100, LX_CONFIG_UI_COLUMN_INTEGER, true);
	lx_config_ui_add_column(x, _lxsym_universe_config_priority, gensym("Priority"),
	                        60, LX_CONFIG_UI_COLUMN_INTEGER, false);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol_interface, gensym("Interface"),
	                        150, LX_CONFIG_UI_COLUMN_MENU, false);
	lx_config_ui_add_column(x, _lxsym_universe_config_unicast_addresses, gensym("Destination Addresses"),
	                        200, LX_CONFIG_UI_COLUMN_TEXT_LIST, false);

	attr_dictionary_process(x, d);

	x->selected_row = NULL;
	x->is_disable_external_refresh = false;

	lx_config_ui_refresh_data(x);

	jbox_ready(&x->box);

	jdataview_resort(x->dataview);

	return x;
}

void lx_config_ui_free(t_lx_config_ui* x)
{
	lx_service_unsubscribe((t_object*)x, x->service);

	jbox_free(&x->box);
	object_free(x->dataview);
}



void lx_config_ui_new_patcherview(t_lx_config_ui* x, t_object* patcherview)
{
	x->patcherview = patcherview;
	jdataview_patchervis(x->dataview, patcherview, (t_object*)x);
}

void lx_config_ui_free_patcherview(t_lx_config_ui* x, t_object* patcherview)
{
	x->patcherview = NULL;
	jdataview_patcherinvis(x->dataview, patcherview);
}


t_max_err lx_config_ui_notify(t_lx_config_ui* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
	if (sender == x->service && msg == _lxsym_service_notify_universes_updated)
	{
		if (!x->is_disable_external_refresh)
			lx_config_ui_refresh_data(x);

		return MAX_ERR_NONE;
	}
	else
	{
		return jbox_notify((t_jbox*)x, s, msg, sender, data);
	}
}

void lx_config_ui_add_universe(t_lx_config_ui* x)
{
	x->is_disable_external_refresh = true;

	t_lx_universe_config* base_config = NULL;
	if (!x->selected_row || hashtab_lookup(x->service->universes, (t_symbol*)x->selected_row, (t_object**)&base_config) != MAX_ERR_NONE)
	{
		t_symbol* name = lx_helpers_hashtab_get_last(x->service->universes, _lxsym_universe_config_internal_universe);
		hashtab_lookup(x->service->universes, name, (t_object**)&base_config);
	}

	t_lx_universe_config* config = lx_universe_config_clone(x->service, base_config);
	if (base_config)
		lx_universe_config_increment(config, 1);

	lx_service_universes_add(x->service, config);

	jdataview_addrow(x->dataview, config->name);
	jdataview_selectcellinview(x->dataview, x->patcherview, _lxsym_universe_config_label, config->name);

	x->is_disable_external_refresh = false;
}

void lx_config_ui_remove_universe(t_lx_config_ui* x)
{
	x->is_disable_external_refresh = true;

	if (x->selected_row)
	{
		t_lx_universe_config* config;
		if (hashtab_lookup(x->service->universes, (t_symbol*)x->selected_row, (t_object**)&config) == MAX_ERR_NONE)
		{
			x->selected_row = lx_helpers_hashtab_get_next(x->service->universes, config->name,
			                                              _lxsym_universe_config_internal_universe);
			jdataview_deleterow(x->dataview, config->name);
			lx_service_universes_remove(x->service, config);
		}
	}

	x->is_disable_external_refresh = false;
}

void lx_config_ui_get_cell_text(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, char* text, long max_length)
{
	t_lx_universe_config* config;
	if (hashtab_lookup(x->service->universes, (t_symbol*)rr, (t_object**)&config) != MAX_ERR_NONE)
		return;

	long argc = 0;
	t_atom* argv = NULL;

	if (object_attr_getvalueof(config, col_name, &argc, &argv) == MAX_ERR_NONE)
	{
		long text_length = 0;
		char* item_text = NULL;

		atom_gettext(argc, argv, &text_length, &item_text, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
		if (item_text && item_text[0])
		{
			strncpy(text, item_text, max_length - 1);
			sysmem_freeptr(item_text);
		}
	}
}

void lx_config_ui_get_cell_value(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long* argc, t_atom* argv)
{
	t_lx_universe_config* config;
	if (hashtab_lookup(x->service->universes, (t_symbol*)rr, (t_object**)&config) != MAX_ERR_NONE)
		return;

	long ac = 0;
	t_atom* av = NULL;

	if (object_attr_getvalueof(config, col_name, &ac, &av) == MAX_ERR_NONE)
	{
		*argc = 1;
		*argv = *av;
	}
}

void lx_config_ui_get_cell_menu(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long* argc, t_atom* argv, char** enabled, long* current_item_index)
{
	t_lx_universe_config* config;
	if (hashtab_lookup(x->service->universes, (t_symbol*)rr, (t_object**)&config) != MAX_ERR_NONE)
		return;

	long ac = 0;
	t_atom* av = NULL;

	if (col_name == _lxsym_universe_config_type)
	{
		lx_universe_type_get_list(config->protocol, &ac, &av);
	}
	else if (col_name == _lxsym_universe_config_protocol)
	{
		lx_universe_protocol_get_list(&ac, &av);
	}
	else if (col_name == _lxsym_universe_config_protocol_mode)
	{
		lx_universe_protocol_mode_get_list(config->type, config->protocol, &ac, &av);
	}
	else if (col_name == _lxsym_universe_config_protocol_interface)
	{
		lx_universe_protocol_interface_get_list(x->service, config->protocol, &ac, &av);
	}

	*argc = ac;
	*enabled = sysmem_newptr(ac);

	for (int i = 0; i < ac; ++i)
	{
		*(argv + i) = *(av + i);
		(*enabled)[i] = true;
	}
}

void lx_config_ui_set_value(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long argc, t_atom* argv)
{
	t_lx_universe_config* config;
	if (hashtab_lookup(x->service->universes, (t_symbol*)rr, (t_object**)&config) != MAX_ERR_NONE)
		return;

	object_attr_setvalueof(config, col_name, argc, argv);

	jdataview_redrawrow(x->dataview, rr);
}

void lx_config_ui_selected_row(t_lx_config_ui* x, t_rowref* rr)
{
	x->selected_row = (t_symbol*)rr;
}


void lx_config_ui_toggle_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label)
{
	*comp_type = JCOLUMN_COMPONENT_CHECKBOX;
}

void lx_config_ui_menu_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label)
{
	*comp_type = JCOLUMN_COMPONENT_MENU;
	*options = JCOLUMN_MENU_SELECT;
}

void lx_config_ui_text_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label)
{
	*comp_type = JCOLUMN_COMPONENT_TEXTEDITOR;
	*options = JCOLUMN_TEXT_ONESYMBOL;
}

void lx_config_ui_text_list_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label)
{
	*comp_type = JCOLUMN_COMPONENT_TEXTEDITOR;
	*options = JCOLUMN_TEXT_COMMASEPARATED;
}

void lx_config_ui_integer_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label)
{
	*comp_type = JCOLUMN_COMPONENT_TEXTEDITOR;
}



void lx_config_ui_add_column(t_lx_config_ui* x, t_symbol* name, t_symbol* label, long width,
							 e_lx_config_ui_column_kind kind, t_bool is_numeric)
{
	t_object* column = jdataview_addcolumn(x->dataview, name, NULL, true);
	jcolumn_setlabel(column, label);
	jcolumn_setwidth(column, width);
	jcolumn_setnumeric(column, is_numeric);
	jcolumn_setdraggable(column, false);

	switch (kind)
	{
		case LX_CONFIG_UI_COLUMN_TEXT:
			jcolumn_setrowcomponentmsg(column, gensym("text_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;

		case LX_CONFIG_UI_COLUMN_TEXT_LIST:
			jcolumn_setrowcomponentmsg(column, gensym("text_list_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;

		case LX_CONFIG_UI_COLUMN_TOGGLE:
			jcolumn_setcheckbox(column, _sym_setvalue);
			jcolumn_setrowcomponentmsg(column, gensym("toggle_component"));
			break;

		case LX_CONFIG_UI_COLUMN_MENU:
			jcolumn_setrowcomponentmsg(column, gensym("menu_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;

		case LX_CONFIG_UI_COLUMN_INTEGER:
			jcolumn_setrowcomponentmsg(column, gensym("integer_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;

		case LX_CONFIG_UI_COLUMN_STATIC:
			break;
	}
}

void lx_config_ui_refresh_data(t_lx_config_ui* x)
{
	jdataview_clear(x->dataview);

	long key_count;
	t_symbol** keys = NULL;

	if (hashtab_getkeys(x->service->universes, &key_count, &keys) == MAX_ERR_NONE)
	{
		jdataview_addrows(x->dataview, key_count, (t_rowref*)keys);

		if (keys)
			sysmem_freeptr(keys);
	}

	jdataview_sort(x->dataview, _lxsym_universe_config_internal_universe, 1);
}