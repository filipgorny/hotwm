#pragma once
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include "controller.h"
#include "keymap.h"
#include "session.h"

#include <lua.h>


typedef struct {
    char* name;
    int (*func)(lua_State* L);
} ScriptingFunction;

typedef struct {
    char* name;
    int functions_count;
    ScriptingFunction* functions[];
} ScriptingModule;

typedef struct ScriptingContextBinding ScriptingContextBinding;
struct ScriptingContextBinding {
    char* name;
    void* value;
    ScriptingContextBinding *next;
};

typedef struct {
    int modules_count;
    ScriptingContextBinding *bindings;
    ScriptingModule *modules[];
} ScriptingContext;


ScriptingContext *scripting_create_context();
void scripting_run(ScriptingContext *context, char *filename);

void scripting_declare_binding(ScriptingContext *context, char *name, void *value);
ScriptingContextBinding* scripting_get_binding(ScriptingContext *context, char* name);

ScriptingModule *scripting_create_module(char *name);
void scripting_declare_function(ScriptingModule *module, char *name, int (*func)(lua_State* L));

void scripting_register_module(ScriptingContext *context, ScriptingModule* (*factory)(ScriptingContext* context));

 
