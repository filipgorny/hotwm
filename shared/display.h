#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <xcb/xcb.h>

typedef struct {
    xcb_screen_t *screen;
} Display;

Display *display_create(xcb_screen_t *screen);

#ifdef __cplusplus
}
#endif

