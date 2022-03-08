#include "decorator.h"
#include "draw.h"
#include "log.h"
#include "window.h"

#include <stdlib.h>
#include <xcb/xcb.h>

Decorator *decorator_initialize(xcb_connection_t *conn, xcb_screen_t *screen) {
  Decorator *decorator = malloc(sizeof(Decorator));
  decorator->conn = conn;
  decorator->screen = screen;
  decorator->root = &screen->root;

  return decorator;
}

void decorator_decorate_window(Decorator *decorator, Window *window) {
  xcb_window_t parent_window = xcb_generate_id(decorator->conn);

  xcb_create_window(decorator->conn, decorator->screen->root_depth,
                    parent_window, *decorator->root, 10, 20, 900, 500, 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    decorator->screen->root_visual, 0, NULL);

  xcb_reparent_window(decorator->conn, *window->window, parent_window,
                      WINDOW_PADDING, TITLE_BAR_HEIGHT);

  window->subwindow = window->window;
  window->window = &parent_window;

  window->draw = draw_init(decorator->conn, decorator->screen, window->window);

  xcb_map_window(decorator->conn, *window->window);

  window_update(window);

  decorator_update_window(decorator, window);

  xcb_flush(decorator->conn);
}

void decorator_update_window(Decorator *decorator, Window *window) {
  log_info("Decorator", "Drawing decoration of parent window");

  xcb_window_t win = *window->window;

  draw_rect(window->draw, win, 0, 0, 800 + WINDOW_PADDING * 2,
            600 + WINDOW_PADDING * 2 + TITLE_BAR_HEIGHT, 0x00444444);
  draw_text(window->draw, win, 30, 50, "TO JEST PRZYKLADOWY TEKST", 0x00ffff00,
            0x0000ff00, "Roboto");

  int subwindow_x = 1;
  int subwindow_y = 1 + TITLE_BAR_HEIGHT + 1;
  int subwindow_width = window->width - 2;
  int subwindow_height = window->height - 2 - TITLE_BAR_HEIGHT - 1;

  xcb_configure_window(window->conn, *window->subwindow,
                       XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                       (uint32_t[]){subwindow_width, subwindow_height});

  xcb_configure_window(window->conn, *window->subwindow,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
                       (uint32_t[]){subwindow_x, subwindow_y});
}

void decorator_refresh(Decorator *decorator, Client *clients) {
  Client *c = clients;

  while (c) {
    decorator_update_window(decorator, c->window);

    c = c->next;
  }
}
