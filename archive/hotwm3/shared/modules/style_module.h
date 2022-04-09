#include "scripting.h"
#include "style.h"

#include <lualib.h>
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

Style *style;

const char* hexchars = "0123456789abcdef";


static int __get_hex_value(char c) {
    for (int i = 0; i < strlen(hexchars); i++) {
        if (hexchars[i] == c) {
            return i;
        }
    }
    
    return -1;
}

static void _style_set_value(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    const double value = luaL_checknumber(L, 2);

    style_define(style, name, value);
}

static void _style_declare(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_pushnil(L);

    int i = 0;

    while (lua_next(L, 1) != 0) {
        const char *name = luaL_checkstring(L, -2);
        const double value = luaL_checknumber(L, -1);

        printf("Defining style value %s = %f\n", name, value);

        style_define(style, name, value);

        lua_pop(L, 1);


        i++;
    }

    printf("Defined %d style values\n", i);
}

static int _style_test(lua_State *L) {
    return 1;
}

static u_int32_t _style_color(lua_State *L) {
   const char* value = luaL_checkstring(L, 1);

    u_int32_t v;
    u_int32_t color = 0;
    int length = strlen(value);

    int i;
    char c;
    int p;

    for (i = 0; i < length - 1; i++) {
       c = value[length - i - 1];

       v = __get_hex_value(c);
       p = pow(0xff, i) * v;

       color += p;
    }

    return color;
}

ScriptingModule * module_style(ScriptingContext *context) {
    printf("module\n");
    style = scripting_get_binding(context, "style")->value;
    printf("style: %d\n", style);

    ScriptingModule *module = scripting_create_module("style");

    scripting_declare_function(module, "set_value", _style_set_value);
    scripting_declare_function(module, "set", _style_declare);
    scripting_declare_function(module, "color", _style_color);
    scripting_declare_function(module, "test", _style_test);

    return module;
}
