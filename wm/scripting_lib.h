#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include <X11/keysym.h>

int _wm_map_key(lua_State *L) {
	int function = luaL_ref(L, LUA_REGISTRYINDEX);
	char* key = luaL_checkstring(L, 1);
    xcb_mod_mask_t modkey;
    char* t;
    //char* key_splitted = strtok(key, "-");

    char* key_char = malloc(2);

    int i = 0;
    bool enable_character_lookup = false;
    bool key_char_defined = false;

    for (i=0;i<strlen(key);i++) {
        printf("KUR: %c\n", key[i]);

        if (enable_character_lookup) {
            key_char[0] = key[i];
            key_char[1] = '\0';
       printf("Defined keychar: %s\n", key_char); 

       key_char_defined = true;
        }

        if (key[i] == 'S') {
            modkey = XCB_MOD_MASK_1;

            printf("Defined modmask: %d\n", modkey);
        }

        if (key[i] == '-') {
            enable_character_lookup = true;
        }
    }

    if (key_char_defined) {
        printf("keychar: %s\n", key_char);
    	KeySymbol keysymbol = scripting_get_keysymbol(key_char);

        printf("Keysymbol: %d\n", keysymbol);

	    scripting_register_keybind(current_scripting_engine, keysymbol, modkey, function);
    }

	return 0;
}

void _wm_next_client(lua_State *L) {
    printf("NEXT\n");

    Client *c = current_session->current_desktop->current_client;

    if (c->next) {
        current_session->current_desktop->current_client = c->next;
    
        refresh();

        return;
    }

    if (current_session->current_desktop->clients) {
        current_session->current_desktop->current_client = current_session->current_desktop->clients;

        refresh();
        
        return;
    }

}

void _wm_prev_client(lua_State *L) {
    printf("PREV\n");
    
    Client *c = current_session->current_desktop->current_client;

    if (c->prev) {
        current_session->current_desktop->current_client = c->prev;
   
        refresh();

        return;
    }

    while (c) {
        if (c->prev == NULL) {
            Client *cc = current_session->current_desktop->current_client;

            while (cc) {
                if (cc->next == NULL) {
                    current_session->current_desktop->current_client = cc;

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
