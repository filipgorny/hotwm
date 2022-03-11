#pragma once
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include "session.h"

#define KEYSYMBOLS_LENGTH 90

typedef struct {
	char *symbol;
	xcb_keysym_t keysym;
} KeySymbol;

typedef struct Keybind Keybind;
struct Keybind {
	KeySymbol key;
	xcb_mod_mask_t mod;
	int function;
	Keybind *next;
};

typedef struct {
	Keybind *keybindings;
} ScriptingEngine;

ScriptingEngine *scripting_create_engine();
void scripting_run(ScriptingEngine *engine, char *filename, Session *session);
void scripting_register_keybind(ScriptingEngine *engine, KeySymbol key, xcb_mod_mask_t modkey, int function);
void scripting_handle_keypress(ScriptingEngine *engine, xcb_connection_t *conn, xcb_key_press_event_t *event);
KeySymbol scripting_get_keysymbol(char *symbol);
