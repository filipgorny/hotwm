#include "scripting.h"
#include "style.h"
#include "spawn.h"

#include <lualib.h>
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>

static void _spawn_spawn(lua_State *L) {
    const char *cmd = luaL_checkstring(L, 1);

    spawn(cmd);
}

ScriptingModule * module_spawn(ScriptingContext *context) {
    ScriptingModule *module = scripting_create_module("spawn");

    scripting_declare_function(module, "spawn", _spawn_spawn);

    return module;
}
