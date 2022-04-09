#include "scripting.h"
#include "controller.h"
#include "log.h"
#include "session.h"

#include <stdlib.h>
#include <string.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <xcb/xproto.h>

ScriptingContext *current_context;
lua_State *L;

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

ScriptingContext *scripting_create_context() {
  ScriptingContext *context = malloc(sizeof(ScriptingContext));
  context->bindings = NULL;
  context->modules_count = 0;

  return context;
}

void scripting_run(ScriptingContext *context, char *filename) {
  L = luaL_newstate();
  luaL_openlibs(L);

  for (int i = 0; i < context->modules_count; i++) {
    struct luaL_Reg mod[context->modules[i]->functions_count + 1];

    for (int ii = 0; ii < context->modules[i]->functions_count; ii++) {
      mod[ii].name = context->modules[i]->functions[ii]->name;
      mod[ii].func = context->modules[i]->functions[ii]->func;
    }

    mod[context->modules[i]->functions_count].name = NULL;
    mod[context->modules[i]->functions_count].func = NULL;

    luaL_newlib(L, mod);

    lua_setglobal(L, context->modules[i]->name);
  }

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

ScriptingModule *scripting_create_module(char *name) {
  ScriptingModule *module = malloc(sizeof(ScriptingModule));
  module->name = name;
  module->functions_count = 0;

  return module;
}

void scripting_declare_function(ScriptingModule *module, char *name,
                                int (*func)(lua_State *L)) {
  ScriptingFunction *function = malloc(sizeof(ScriptingFunction));
  function->name = name;
  function->func = func;

  module->functions[module->functions_count] = function;
  module->functions_count++;
}

void scripting_register_module(
    ScriptingContext *context,
    ScriptingModule *(*factory)(ScriptingContext *context)) {
  ScriptingModule *module = factory(context);
  context->modules[context->modules_count] = module;
  context->modules_count++;
}

void scripting_declare_binding(ScriptingContext *context, char *name,
                               void *value) {
  ScriptingContextBinding *binding = malloc(sizeof(ScriptingContextBinding));
  binding->name = name;
  binding->value = value;
  binding->next = NULL;

  ScriptingContextBinding *b = context->bindings;

  if (b == NULL) {
    context->bindings = binding;
  } else {
    while (b->next) {
      if (b->next == NULL) {
        b->next = binding;

        break;
      }

      b = b->next;
    }
  }
}

ScriptingContextBinding *scripting_get_binding(ScriptingContext *context,
                                               char *name) {
  ScriptingContextBinding *b = context->bindings;

  while (b) {
    if (strcmp(b->name, name) == 0) {
      return b;
    }

    b = b->next;
  }

  return NULL;
}
