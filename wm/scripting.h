#pragma once
#include <xcb/xcb_keysyms.h>

#define KEYSYMBOLS_LENGTH 90

typedef struct {
	char *symbol;
	xcb_keysym_t keysym;
} KeySymbol;

typedef struct Keybind Keybind;
struct Keybind {
	KeySymbol key;
	uint32_t mod;
	int function;
	Keybind *next;
};

typedef struct {
	Keybind *keybindings;
} ScriptingEngine;

ScriptingEngine *scripting_create_engine();
void scripting_run(ScriptingEngine *engine, char *filename);
void scripting_register_keybind(ScriptingEngine *engine, KeySymbol key, int function);
void scripting_handle_keypress(ScriptingEngine *engine, xcb_connection_t *conn, xcb_key_press_event_t *event);
KeySymbol scripting_get_keysymbol(char *symbol);
