#include "window.h"
#include <xcb/xcb.h>

#include "wm.h"

char *window_get_name(Client *client) {
  xcb_window_t window = client->window;
  xcb_connection_t *conn = client->conn;

  xcb_get_property_cookie_t cookie = xcb_get_property(
      conn, 0, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 1024);
  xcb_get_property_reply_t *reply = xcb_get_property_reply(conn, cookie, NULL);
  if (reply == NULL) {
    return NULL;
  }
  if (reply->type == XCB_ATOM_STRING) {
    char *name = xcb_get_property_value(reply);
    return name;
  }
  return NULL;
}
