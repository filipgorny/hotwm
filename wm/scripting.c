#include "scripting.h"
#include "controller.h"
#include "log.h"
#include "session.h"

#include <stdlib.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <xcb/xproto.h>

ScriptingContext *current_context;
lua_State *L;

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

ScriptingContext *scripting_create_context(Controller *controller,
                                           Keymap *keymap, Session *session) {
  ScriptingContext *context = malloc(sizeof(ScriptingContext));
  context->controller = controller;
  context->keymap = keymap;
  context->session = session;

  return context;
}

void scripting_run(ScriptingContext *context, char *filename) {
  L = luaL_newstate();
  luaL_openlibs(L);

  const struct luaL_Reg wmlib[] = {{"map_key", _wm_map_key},
                                   {"next_client", _wm_next_client},
                                   {"prev_client", _wm_prev_client},
                                   {"set_layout", _wm_set_layout},
                                   {"spawn", _wm_spawn},
                                   {NULL, NULL}};

  luaL_newlib(L, wmlib);
  lua_setglobal(L, "wm");

  char *code = read_file(filename);

  if (code != NULL) {
    log_info("Scripting", "Running file");

    if (luaL_loadstring(L, code) == LUA_OK) {
      current_context = context;

      const int pcall = lua_pcall(L, 0, 0, 0);

      if (pcall == LUA_OK) {
        lua_pop(L, lua_gettop(L));
      } else if (pcall == LUA_ERRRUN) {
        printf(lua_tostring(L, -1));
        luaL_traceback(L, L, NULL, 1);
        printf(lua_tostring(L, -1));
      }
    }
  }
}
