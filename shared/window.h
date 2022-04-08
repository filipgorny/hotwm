#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include "draw.h"

typedef struct {
    xcb_connection_t *conn;
    xcb_window_t window, subwindow;
    Draw *draw;
    int x, y;
    int width, height;
    char* title;
    bool hidden;
} Window;

Window *window_create(xcb_connection_t *conn, xcb_screen_t *screen, xcb_window_t root, xcb_window_t window);
void window_update(Window *window);
char* window_find_name(Window *window);
int window_get_type(Window *window);

#ifdef __cplusplus
}
#endif
