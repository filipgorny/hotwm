#pragma once

#include <lualib.h>
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>


static void _keymap_bind(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);
    const char *cmd = luaL_checkstring(L, 1);

    keymap_bind(key, cmd);
}

ScriptingModule * module_keymap(ScriptingContext *context) {
    ScriptingModule *module = scripting_create_module("keymap");

    scripting_declare_function(module, "bind", _keymap_bind);

    return module;
}

