#pragma once

#include <stdbool.h>
#include <xcb/xproto.h>

#include "window.h"

typedef struct {
    int x, y;
    bool button1;
    bool button2;
    bool button3;
    xcb_mod_mask_t modmask;
    Window *window;
} PointerAction;

typedef struct {
    int x, y;
    PointerAction *last_action;
} Pointer;

Pointer *pointer_initialize();
void pointer_update_cords(Pointer *pointer, int x, int y);
void pointer_update_action(Pointer *pointer, bool button1, bool button2, bool button3, xcb_mod_mask_t modmask, Window *window);

