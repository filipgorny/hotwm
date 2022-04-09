#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "arg.h"
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>


typedef struct {
	char *symbol;
	xcb_keysym_t keysym;
} KeySymbol;

typedef struct Keybind Keybind;
struct Keybind {
	KeySymbol key;
	xcb_mod_mask_t modkey;
	char* cmd;
    Arg arg;
	Keybind *next;
};

typedef struct {
    Keybind *keybinds;
} Keymap;


Keymap *keymap_create();
void keymap_bind(Keymap *keymap, char *key, char *cmd, Arg arg);
Keybind *keymap_get_keybind(Keymap *keymap, xcb_keysym_t keysym, xcb_mod_mask_t modkey);

#ifdef __cplusplus
}
#endif
