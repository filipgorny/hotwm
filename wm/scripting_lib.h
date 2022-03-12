#include "arg.h"

int _wm_map_key(lua_State *L) {
	int function = luaL_ref(L, LUA_REGISTRYINDEX);
	char* key = luaL_checkstring(L, 1);
    
    Arg arg = { .i = function };

    keymap_bind(current_context->keymap, key, "script:callback", arg);

	return 0;
}

void _wm_next_client(lua_State *L) {
    Client *c = current_context->session->current_desktop->current_client;

    if (c->next) {
        current_context->session->current_desktop->current_client = c->next;
    
        refresh();

        return;
    }

    if (current_context->session->current_desktop->clients) {
        current_context->session->current_desktop->current_client = current_context->session->current_desktop->clients;

        refresh();
        
        return;
    }

}

void _wm_prev_client(lua_State *L) {
    Client *c = current_context->session->current_desktop->current_client;

    if (c->prev) {
        current_context->session->current_desktop->current_client = c->prev;
   
        refresh();

        return;
    }

    while (c) {
        if (c->prev == NULL) {
            Client *cc = current_context->session->current_desktop->current_client;

            while (cc) {
                if (cc->next == NULL) {
                    current_context->session->current_desktop->current_client = cc;

                    refresh();

                    return;
                }

                cc = cc->next;
            }

            return;
        }

        c = c->prev;
    }
}

void _wm_set_layout(lua_State *L) {
    char* layout_name = luaL_checkstring(L, 1);

    session_select_layout(current_context->session, layout_name);

    refresh();
}

void _wm_spawn(lua_State *L) {
    char* process = luaL_checkstring(L, 1);

    Arg arg = {.v = process };

    controller_run(current_context->controller, "spawn", arg);
}

void _wm_style_set(lua_State *L) {
    
}
