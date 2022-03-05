#include "window.h"
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

Window *window_create(xcb_connection_t *conn, xcb_window_t *window) {
  Window *w = malloc(sizeof(Window));
  w->conn = conn;
  w->window = window;

  WindowProperties *properties = malloc(sizeof(WindowProperties));
  w->properties = properties;

  return w;
}

void window_update(Window *window) {
  xcb_get_geometry_cookie_t cookie;
  xcb_get_geometry_reply_t *reply;

  cookie = xcb_get_geometry(window->conn, *window->window);

  reply = xcb_get_geometry_reply(window->conn, cookie, NULL);

  window->properties->x = reply->x;
  window->properties->y = reply->y;
  window->properties->width = reply->width;
  window->properties->height = reply->height;
}
