#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

#include "background.h"
#include "draw.h"
#include "main.h"

#include "config.h"

void background_draw(xcb_screen_t *screen, xcb_window_t *root) {
  draw_rect(root, 0, 0, screen->width_in_pixels, screen->height_in_pixels,
            COLOR_DESKTOP);
}
