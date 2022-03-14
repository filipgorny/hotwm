#include <stdio.h>
#include <xcb/xcb.h>

#include "window.h"

xcb_connection_t *conn;
const struct xcb_setup_t *setup;
xcb_screen_t *screen;

xcb_window_t window;

xcb_window_t create_window() {
  window = xcb_create_window(conn, XCB_COPY_FROM_PARENT, screen->root, 0, 0,
                             screen->width_in_pixels, screen->height_in_pixels,
                             0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                             screen->root_visual, 0, NULL);
}

int main() {
  conn = xcb_connect(NULL, NULL);
  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;
}
