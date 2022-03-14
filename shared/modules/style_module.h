#include "scripting.h"
#include "style.h"

#include <lualib.h>
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>

static void _style_set_value(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    const int value = luaL_checkinteger(L, 2);

    style_define(style, name, value);
}

static void _style_declare(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_pushnil(L);

    int i = 0;

    while (lua_next(L, 1) != 0) {
        const char *name = luaL_checkstring(L, -2);
        const int value = luaL_checkinteger(L, -1);

        printf("Defining style value %s = %d\n", name, value);

        style_define(style, name, value);

        lua_pop(L, 1);


        i++;
    }

    printf("Defined %d style values\n", i);
}

static void _style_color(lua_State *L) {
    const char* value = luaL_checkstring(L, 1);


    
}

ScriptingModule * module_style(ScriptingContext *context) {
    ScriptingModule *module = scripting_create_module("style");

    scripting_declare_function(module, "set_value", _style_set_value);
    scripting_declare_function(module, "set", _style_declare);

    return module;
}
