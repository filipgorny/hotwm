#pragma once

#include "grid.h"
#include <xcb/xcb.h>

typedef struct {
    xcb_screen_t *screen;
} Display;

Display *display_create(xcb_screen_t *screen);
