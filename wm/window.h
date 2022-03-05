#pragma once

#include <xcb/xcb.h>
#include <xcb/xproto.h>

typedef struct {
    int x, y;
    int width, height;
} WindowProperties;

typedef struct {
    xcb_connection_t *conn;
    xcb_window_t *window;
    xcb_window_t *child_window;
    WindowProperties *properties;
} Window;

Window *window_create(xcb_connection_t *conn, xcb_window_t *window);
void window_update(Window *window);
