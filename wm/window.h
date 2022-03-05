#pragma once

#include <xcb/xcb.h>

typedef struct {
    int x, y;
    int width, height;
    xcb_window_t *window;
} Window;

Window *window_create(xcb_window_t *window);
