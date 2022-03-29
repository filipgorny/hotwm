#include "panel.h"

#include <stdlib.h>

Panel *panel_create(xcb_connection_t conn, xcb_screen_t screen, xcb_window_t root, int x, int y, int w, int h) {
  Panel *p = malloc(sizeof(Panel));

  xcb_window_t window = xcb_generate_id(conn);

  xcb_create_window(conn, screen->root_depth, w->window, root, w->x, w->y,
                    w->width, w->height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, 0, NULL);

  xcb_reparent_window(conn, w->subwindow, w->window, 0, 0);

  xcb_map_window(conn, w->window);

  xcb_flush(conn);

  return p;
}
