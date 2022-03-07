#include "window.h"
#include "log.h"
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

Window *window_create(xcb_connection_t *conn, xcb_window_t *window) {
  Window *w = malloc(sizeof(Window));
  w->conn = conn;
  w->window = window;

  return w;
}

void window_update(Window *window) {}

void window_resize(Window *window, int width, int height) {
  xcb_window_t w = *window->window;

  xcb_configure_window(window->conn, w,
                       XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                       (uint32_t[]){width, height});

  window_update(window);
}

void window_position(Window *window, int x, int y) {
  xcb_window_t w = *window->window;

  xcb_configure_window(window->conn, w,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
                       (uint32_t[]){x, y});

  window_update(window);
}
