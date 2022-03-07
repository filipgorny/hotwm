#pragma once

#include "gui.h"
#include "window.h"
#include "client.h"
#include <xcb/xproto.h>
#include <xcb/xcb.h>

#define WINDOW_PADDING 2
#define TITLE_BAR_HEIGHT 20


typedef struct {
    xcb_connection_t *conn;
    xcb_screen_t *screen;
    xcb_window_t *root;
} Decorator;

Decorator *decorator_initialize(xcb_connection_t *conn, xcb_screen_t *screen);
void decorator_decorate_window(Decorator *decorator, Window *window);
void decorator_update_window(Decorator *decorator, Window *window);
void decorator_refresh(Decorator *decorator, Client *clients);
