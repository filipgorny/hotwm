#pragma once

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include "draw.h"

typedef struct {
    xcb_connection_t *conn;
    xcb_window_t *window, *subwindow;
    Draw *draw;
    int x, y;
    int width, height;
} Window;

Window *window_create(xcb_connection_t *conn, xcb_window_t *window);
void window_update(Window *window);
