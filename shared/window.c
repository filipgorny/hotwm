#include "window.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#define WINDOW_UNTITLED_TEXT "<Untitled>"

Window *window_create(xcb_connection_t *conn, xcb_screen_t *screen,
                      xcb_window_t root, xcb_window_t window) {
  Window *w = malloc(sizeof(Window));
  w->conn = conn;
  w->subwindow = window;
  w->title = "";
  w->hidden = false;

  w->x = screen->width_in_pixels / 2 - screen->width_in_pixels / 4;
  w->y = screen->height_in_pixels / 2 - screen->height_in_pixels / 4,
  w->width = screen->width_in_pixels / 4;

  w->height = screen->height_in_pixels / 4;

  w->window = xcb_generate_id(conn);

  xcb_create_window(conn, screen->root_depth, w->window, root, w->x, w->y,
                    w->width, w->height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, 0, NULL);

  xcb_reparent_window(conn, w->subwindow, w->window, 0, 0);

  xcb_map_window(conn, w->window);

  xcb_flush(conn);

  return w;
}

void window_update(Window *window) {
  xcb_window_t w = window->window;

  xcb_configure_window(window->conn, w,
                       XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                       (int[]){window->width, window->height});

  xcb_configure_window(window->conn, w,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
                       (int[]){window->x, window->y});

  if (window->hidden) {
    xcb_unmap_window(window->conn, w);
  } else {
    xcb_map_window(window->conn, w);
  }

  xcb_flush(window->conn);
}

char *window_find_name(Window *window) {
  xcb_get_property_cookie_t cookie =
      xcb_get_property(window->conn, 0, window->subwindow, XCB_ATOM_WM_NAME,
                       XCB_ATOM_STRING, 0, UINT32_MAX);

  xcb_get_property_reply_t *reply =
      xcb_get_property_reply(window->conn, cookie, NULL);

  if (reply) {
    char *name = xcb_get_property_value(reply);

    if (name && !(name[0] == '\0')) {
      log_info("window", name);

      return name;
    }
  }

  return WINDOW_UNTITLED_TEXT;
}
