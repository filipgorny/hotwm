#pragma once

#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>


typedef struct {
	char *symbol;
	xcb_keysym_t keysym;
} KeySymbol;


typedef struct Keybind Keybind;
struct Keybind {
	KeySymbol key;
	xcb_mod_mask_t mod;
	int function;
	Keybind *next;
};


typedef struct {
	Keybind *keybindings;
} Manager;
