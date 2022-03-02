#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

static void background_draw(xcb_screen_t *screen, xcb_window_t *root);

#endif
