// Copyright (c) 2021 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include <ext.h>
#include <ext_obex.h>
#include <ext_dictobj.h>
#include <ext_globalsymbol.h>
#include <commonsyms.h>
#include <jdataview.h>

#include "lxmax.h"

typedef enum _column_kind
{
	COLUMN_TEXT,
	COLUMN_TEXT_LIST,
	COLUMN_TOGGLE,
	COLUMN_MENU,
	COLUMN_INTEGER

} e_lx_config_ui_column_kind;

typedef struct _lx_config_ui
{
	t_jbox box;
	t_lx_service* service;

	t_object* dataview;

	t_lx_universe_config* selected_row;

} t_lx_config_ui;

static t_class* lx_config_ui_class;

void* lx_config_ui_new(t_symbol* s, short argc, t_atom* argv);
void lx_config_ui_free(t_lx_config_ui* x);

void lx_config_ui_new_patcherview(t_lx_config_ui* x, t_object* patcherview);
void lx_config_ui_free_patcherview(t_lx_config_ui* x, t_object* patcherview);

t_max_err lx_config_ui_notify(t_lx_config_ui* x, t_symbol* s, t_symbol* msg, void* sender, void* data);


void lx_config_ui_add_universe(t_lx_config_ui* x);
void lx_config_ui_remove_universe(t_lx_config_ui* x);
void lx_config_ui_clear_universes(t_lx_config_ui* x);

void lx_config_ui_get_cell_text(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, char* text, long max_length);
void lx_config_ui_get_cell_value(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long* argc, t_atom* argv);
void lx_config_ui_get_cell_menu(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long* argc, t_atom* argv, char** enabled, long* current_item_index);
void lx_config_ui_set_value(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, long argc, t_atom* argv);
void lx_config_ui_selected_row(t_lx_config_ui* x, t_rowref* rr);

void lx_config_ui_toggle_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_menu_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_text_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_text_list_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);
void lx_config_ui_integer_component(t_object* x, t_symbol* col_name, t_rowref rr, long* comp_type, long* options, t_symbol** label);

void lx_config_ui_add_column(t_lx_config_ui* x, t_symbol* name, t_symbol* label, long width, e_lx_config_ui_column_kind kind);
void lx_config_ui_refresh_data(t_lx_config_ui* x);


void ext_main(void* r)
{
	common_symbols_init();
	lx_sym_init();

	t_class* c = class_new("lx.config.ui", (method)lx_config_ui_new, (method)lx_config_ui_free, sizeof(t_lx_config_ui), 
		(method)NULL,A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_COLOR);

	class_addmethod(c, (method)lx_config_ui_new_patcherview, "newpatcherview", A_CANT, 0);
	class_addmethod(c, (method)lx_config_ui_free_patcherview, "freepatcherview", A_CANT, 0);

	class_addmethod(c, (method)lx_config_ui_notify, "notify", A_CANT, 0);

	class_addmethod(c, (method)lx_config_ui_add_universe, "add_universe", 0);
	class_addmethod(c, (method)lx_config_ui_remove_universe, "remove_universe", 0);
	class_addmethod(c, (method)lx_config_ui_clear_universes, "clear_universes", 0);

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

	x->service = (t_lx_service*)globalsymbol_reference((t_object*)x, LX_SERVICE_SYMBOL_NAME, LX_SERVICE_CLASS_NAME);
	if (x->service == NULL)
	{
		object_error((t_object*)x, "Failed to find active LXMax service");
		object_free(x);
		return NULL;
	}
	object_attach_byptr(x, x->service);

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

	lx_config_ui_add_column(x, _lxsym_universe_config_label, gensym("Label"), 200, COLUMN_TEXT);
	lx_config_ui_add_column(x, _lxsym_universe_config_type, gensym("Type"), 70, COLUMN_MENU);
	lx_config_ui_add_column(x, _lxsym_universe_config_enable, gensym("On"), 40, COLUMN_TOGGLE);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol, gensym("Protocol / Device"), 100, COLUMN_MENU);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol_mode, gensym("Mode"), 100, COLUMN_MENU);
	lx_config_ui_add_column(x, _lxsym_universe_config_internal_universe, gensym("Internal Universe"), 100, COLUMN_INTEGER);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol_universe, gensym("Protocol Universe"), 100, COLUMN_INTEGER);
	lx_config_ui_add_column(x, _lxsym_universe_config_priority, gensym("Priority"), 70, COLUMN_INTEGER);
	lx_config_ui_add_column(x, _lxsym_universe_config_protocol_interface, gensym("Interface"), 200, COLUMN_MENU);
	lx_config_ui_add_column(x, _lxsym_universe_config_unicast_addresses, gensym("Destination Addresses"), 200, COLUMN_TEXT_LIST);

	attr_dictionary_process(x, d);

	x->selected_row = NULL;

	lx_config_ui_refresh_data(x);

	jbox_ready(&x->box);
	return x;
}

void lx_config_ui_free(t_lx_config_ui* x)
{
	object_detach_byptr(x, x->service);
	globalsymbol_dereference((t_object*)x, LX_SERVICE_SYMBOL_NAME, LX_SERVICE_CLASS_NAME);

	jbox_free(&x->box);
	object_free(x->dataview);
}



void lx_config_ui_new_patcherview(t_lx_config_ui* x, t_object* patcherview)
{
	jdataview_patchervis(x->dataview, patcherview, (t_object*)x);
}

void lx_config_ui_free_patcherview(t_lx_config_ui* x, t_object* patcherview)
{
	jdataview_patcherinvis(x->dataview, patcherview);
}


t_max_err lx_config_ui_notify(t_lx_config_ui* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
	if (sender == x->service && msg == _lxsym_service_notify_universes_updated)
	{
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
	if (x->selected_row)
	{
		const long index = linklist_objptr2index(x->service->universes, x->selected_row);
		lxmax_service_universes_add(x->service, lx_universe_config_new(x->service, NULL), index);
	}
	else
	{
		lxmax_service_universes_add(x->service, lx_universe_config_new(x->service, NULL), -1);
	}
}

void lx_config_ui_remove_universe(t_lx_config_ui* x)
{
	if (x->selected_row)
		lxmax_service_universes_remove(x->service, x->selected_row);
}

void lx_config_ui_clear_universes(t_lx_config_ui* x)
{
	lxmax_service_universes_clear(x->service);
}



void lx_config_ui_get_cell_text(t_lx_config_ui* x, t_symbol* col_name, t_rowref rr, char* text, long max_length)
{
	if (object_isnogood(rr))
		return;

	t_lx_universe_config* config = (t_lx_universe_config*)rr;

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
	if (object_isnogood(rr))
		return;

	t_lx_universe_config* config = (t_lx_universe_config*)rr;

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
	if (object_isnogood(rr))
		return;

	t_lx_universe_config* config = (t_lx_universe_config*)rr;

	long ac = 0;
	t_atom* av = NULL;

	if (col_name == _lxsym_universe_config_type)
	{
		lx_universe_type_get_list(&ac, &av);
	}
	else if (col_name == _lxsym_universe_config_protocol)
	{
		lx_universe_protocol_get_list(&ac, &av);
	}
	else if (col_name == _lxsym_universe_config_protocol_mode)
	{
		lx_universe_config_get_modes(config, &ac, &av);
	}
	else if (col_name == _lxsym_universe_config_protocol_interface)
	{
		lx_network_adapter_get_menu_items(x->service->network_adapters, &ac, &av);
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
	if (object_isnogood(rr))
		return;

	t_lx_universe_config* config = (t_lx_universe_config*)rr;
	object_attr_setvalueof(config, col_name, argc, argv);

	jdataview_redrawrow(x->dataview, rr);
}

void lx_config_ui_selected_row(t_lx_config_ui* x, t_rowref* rr)
{
	if (object_isnogood(rr))
		x->selected_row = NULL;
	else
		x->selected_row = (t_lx_universe_config*)rr;
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



void lx_config_ui_add_column(t_lx_config_ui* x, t_symbol* name, t_symbol* label, long width, e_lx_config_ui_column_kind kind)
{
	t_object* column = jdataview_addcolumn(x->dataview, name, NULL, true);
	jcolumn_setlabel(column, label);
	jcolumn_setwidth(column, width);

	switch (kind)
	{
		case COLUMN_TEXT:
			jcolumn_setrowcomponentmsg(column, gensym("text_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;

		case COLUMN_TEXT_LIST:
			jcolumn_setrowcomponentmsg(column, gensym("text_list_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;

		case COLUMN_TOGGLE:
			jcolumn_setcheckbox(column, _sym_setvalue);
			jcolumn_setrowcomponentmsg(column, gensym("toggle_component"));
			break;

		case COLUMN_MENU:
			jcolumn_setrowcomponentmsg(column, gensym("menu_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;

		case COLUMN_INTEGER:
			jcolumn_setrowcomponentmsg(column, gensym("integer_component"));
			jcolumn_setvaluemsg(column, _sym_setvalue, NULL, NULL);
			break;
	}
}

void lx_config_ui_refresh_data(t_lx_config_ui* x)
{
	jdataview_clear(x->dataview);

	const long universe_count = (long)linklist_getsize(x->service->universes);

	t_rowref* row_refs = (t_rowref*)sysmem_newptr(sizeof(t_rowref) * universe_count);

	t_rowref* row_ref = row_refs;
	t_lx_universe_config* config = (t_lx_universe_config*)linklist_getindex(x->service->universes, 0);
	while(config)
	{
		*row_ref++ = (t_rowref*)config;

		linklist_next(x->service->universes, config, (void**)&config);
	}

	jdataview_addrows(x->dataview, universe_count, row_refs);
	sysmem_freeptr(row_refs); 
}