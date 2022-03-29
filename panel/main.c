#include "panel.h"
#include "draw.h"

#include <stdio.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

xcb_connection_t *conn;
const xcb_setup_t *setup;
xcb_drawable_t root;
xcb_screen_t *screen;

Panel * panel;

int main() {
  conn = xcb_connect(NULL, NULL);
  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;
  root = screen->root;

	panel = panel_create(conn, screen, root, 0, screen->height_in_pixels - PANEL_HEIGHT, screen->width_in_pixels, PANEL_HEIGHT);
}
