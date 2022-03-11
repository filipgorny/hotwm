#include "scripting.h"

#include <X11/keysym.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

KeySymbol keysymbols[KEYSYMBOLS_LENGTH] = {
		{ "a", XK_a },
		{ "b", XK_b },
		{ "c", XK_c },
		{ "d", XK_d },
		{ "e", XK_e },
		{ "f", XK_f },
		{ "g", XK_g },
		{ "h", XK_h },
		{ "i", XK_i },
		{ "j", XK_j },
		{ "k", XK_k },
		{ "l", XK_l },
		{ "m", XK_m },
		{ "n", XK_n },
		{ "o", XK_o },
		{ "p", XK_p },
		{ "q", XK_q },
		{ "r", XK_r },
		{ "s", XK_s },
		{ "t", XK_t },
		{ "u", XK_u },
		{ "v", XK_v },
		{ "w", XK_w },
		{ "x", XK_x },
		{ "y", XK_y },
		{ "z", XK_z },
		{ "1", XK_1 },
		{ "2", XK_2 },
		{ "3", XK_3 },
		{ "4", XK_4 },
		{ "5", XK_5 },
		{ "6", XK_6 },
		{ "7", XK_7 },
		{ "8", XK_8 },
		{ "9", XK_9 },
		{ "0", XK_0 },
		{ "F1", XK_F1 },
		{ "F2", XK_F2 },
		{ "F3", XK_F3 },
		{ "F4", XK_F4 },
		{ "F5", XK_F5 },
		{ "F6", XK_F6 },
		{ "F7", XK_F7 },
		{ "F8", XK_F8 },
		{ "F9", XK_F9 },
		{ "F10", XK_F10 },
		{ "F11", XK_F11 },
		{ "F12", XK_F12 },
		{ "R", XK_Return },
		{ "E", XK_Escape },
		{ "T", XK_Tab },
		{ "`", XK_grave },
		{ "-", XK_minus },
		{ "=", XK_equal },
		{ "[", XK_bracketleft },
		{ "]", XK_bracketright },
		{ "\\", XK_backslash },
		{ ";", XK_semicolon },
		{ "'", XK_apostrophe },
		{ ",", XK_comma },
		{ ".", XK_period },
		{ "/", XK_slash },
		{ "space", XK_space },
		{ "up", XK_Up },
		{ "down", XK_Down },
		{ "left", XK_Left },
		{ "right", XK_Right },
		{ "home", XK_Home },
		{ "end", XK_End },
		{ "pageup", XK_Page_Up },
		{ "pagedown", XK_Page_Down },
		{ "insert", XK_Insert },
		{ "delete", XK_Delete },
		{ "backspace", XK_BackSpace },
		{ "enter", XK_Return },
		{ "tab", XK_Tab },
		{ "capslock", XK_Caps_Lock },
		{ "numlock", XK_Num_Lock },
		{ "scrolllock", XK_Scroll_Lock },
		{ "pause", XK_Pause },
		{ "printscreen", XK_Print }
	};

ScriptingEngine *current_scripting_engine;

#include "scripting_lib.h"

char *read_file(char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		log_error("Could not open init file");

		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(size + 1);
	fread(buffer, 1, size, file);
	buffer[size] = '\0';

	fclose(file);

	return buffer;
}

ScriptingEngine *scripting_create_engine() {
	ScriptingEngine *engine = malloc(sizeof(ScriptingEngine));
	engine->keybindings = NULL;

	return engine;
}

void scripting_run(ScriptingEngine *engine, char *filename) {
	  lua_State *L = luaL_newstate();
    luaL_openlibs(L);

		const struct luaL_Reg wmlib[] = {
			{"map_key", _wm_map_key},
			{NULL, NULL}
		};
		
		luaL_newlib(L, wmlib);
		lua_setglobal(L, "wm");

    char * code = read_file(filename);
		
		if (code != NULL) {
			log_info("Scripting", "Running file");

			current_scripting_engine = engine;

    	if (luaL_loadstring(L, code) == LUA_OK) {
				const int pcall = lua_pcall(L, 0, 0, 0);

       	if (pcall == LUA_OK) {
      	    lua_pop(L, lua_gettop(L));
        } else if (pcall == LUA_ERRRUN) {
					log_error(lua_tostring(L, -1));

					luaL_traceback(L, L, NULL, 1);
					log_error(lua_tostring(L, -1));
				}
    	}
		}

    lua_close(L);
    return 0;
}

void scripting_register_keybind(ScriptingEngine *engine, KeySymbol key, int function) {
	printf("Registering keybind\n");

	Keybind *binding = malloc(sizeof(Keybind));
	binding->key = key;
	binding->function = function;
printf("Keybind keysym %d\n", binding->key.keysym);
	Keybind *k = engine->keybindings;
	
	if (!k) {
		engine->keybindings = binding;

		return;
	}

	while (k) {
		if (k->next == NULL || k->next == k) {
			k->next = binding;

			return;
		}

		k = k->next;
	}
}

void scripting_handle_keypress(ScriptingEngine *engine, xcb_connection_t *conn, xcb_key_press_event_t *event) {
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
  xcb_keysym_t keysym;
  keysym =
      (!(keysyms) ? 0 : xcb_key_symbols_get_keysym(keysyms, event->detail, 0));
printf("Keypress: %d\n", keysym);
  if (keysym == NULL) {
    printf("[KEYBOARD] Unknown keysym\n");

    return;
  }

	Keybind *k = engine->keybindings;
printf("Keybinds: %d\n", k->key.keysym);
	while (k) {
		if (k->key.keysym == keysym) {
			printf("[KEYBOARD] Found keybind\n");
	  lua_State *L = luaL_newstate();
    luaL_openlibs(L);

		const struct luaL_Reg wmlib[] = {
			{"map_key", _wm_map_key},
			{NULL, NULL}
		};
		
		luaL_newlib(L, wmlib);
		lua_setglobal(L, "wm");
printf("function %d\n", k->function);

		lua_geti(L, LUA_REGISTRYINDEX, k->function);

				const int pcall = lua_pcall(L, 1, 0, 0);

       	if (pcall == LUA_OK) {
      	    lua_pop(L, lua_gettop(L));
        } else if (pcall == LUA_ERRRUN) {
					log_error(lua_tostring(L, -1));

					luaL_traceback(L, L, NULL, 1);
					log_error(lua_tostring(L, -1));
				}
printf("Pcall returned: %d\n", pcall);
				lua_close(L);
				return;
		return;
		}

		k = k->next;
	}
}

KeySymbol scripting_get_keysymbol(char *symbol) {
	for (int i = 0; i < sizeof(keysymbols) / sizeof(KeySymbol); i++) {
		if (strcmp(keysymbols[i].symbol, symbol) == 0) {
			printf("Found keysym %d\n", keysymbols[i].keysym);
			return keysymbols[i];
		}
	}
}
