#include "decorator.h"
#include "draw.h"
#include "window.h"

#include <stdlib.h>
#include <xcb/xcb.h>

Decorator *decorator_initialize(xcb_connection_t *conn, xcb_screen_t *screen,
                                xcb_window_t *root, Gui *gui) {
  Decorator *decorator = malloc(sizeof(Decorator));
  decorator->gui = gui;
  decorator->conn = conn;
  decorator->screen = screen;
  decorator->root = root;

  return decorator;
}

void decorator_decorate_window(Decorator *decorator, Window *window) {
  xcb_window_t parent_window = xcb_generate_id(decorator->conn);

  xcb_create_window(decorator->conn, decorator->screen->root_depth,
                    parent_window, *decorator->root, 2, 20, 200, 500, 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    decorator->screen->root_visual, 0, NULL);

  xcb_reparent_window(decorator->conn, *window->window, parent_window,
                      WINDOW_PADDING, TITLE_BAR_HEIGHT);

  xcb_map_window(decorator->conn, parent_window);

  xcb_flush(decorator->conn);

  window->child_window = window->window;
  window->window = &parent_window;

  window_update(window);

  decorator_update_window(decorator, window);
}

void decorator_update_window(Decorator *decorator, Window *window) {
  draw_rect(decorator->gui->draw, *window->window, 0, 0,
            window->properties->width + WINDOW_PADDING * 2,
            window->properties->height + WINDOW_PADDING * 2 + TITLE_BAR_HEIGHT,
            0x00444444);
}
