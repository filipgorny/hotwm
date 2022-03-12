#pragma once

#include <stdbool.h>
#include <xcb/xproto.h>

#include "arg.h"
#include "window.h"

typedef struct {
    int x, y;
    bool button1;
    bool button2;
    bool button3;
    xcb_mod_mask_t modmask;
    Window *window;
    bool release;
} PointerUpdate;

typedef struct PointerUpdateBind PointerUpdateBind;
struct PointerUpdateBind {
    bool release;
    bool button1;
    bool button2;
    bool button3;
    xcb_mod_mask_t modmask;
    char* cmd;
    Arg arg;
    PointerUpdateBind *next;
};

typedef struct {
    int x, y;
    PointerUpdate *update, *prev_update;
    PointerUpdateBind *action_binds;
} Pointer;

Pointer *pointer_initialize();
void pointer_update_cords(Pointer *pointer, int x, int y);
void pointer_update_action(Pointer *pointer, bool button1, bool button2, bool button3, xcb_mod_mask_t modmask, Window *window, bool release);
void pointer_map_action(Pointer *pointer, char* action_desc, char* cmd, Arg arg);
PointerUpdateBind *pointer_get_action_bind(Pointer *pointer, bool button1,
                                           bool button2, bool button3,
                                           xcb_mod_mask_t modmask, bool release);
