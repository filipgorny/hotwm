#include "client.h"
#include <stdlib.h>
#include <xcb/xcb.h>

char *client_get_name(Client *client) {

  xcb_atom_t atom = XCB_ATOM_WM_NAME;

  xcb_get_property_cookie_t c = xcb_get_property(
      client->conn, 0, client->window, atom, XCB_GET_PROPERTY_TYPE_ANY, 0, 256);
  xcb_get_property_reply_t *r = xcb_get_property_reply(client->conn, c, NULL);
  if (r) {
    if (xcb_get_property_value_length(r) > 0) {
      char *str = NULL;
      str = xcb_get_property_value(r);

      free(r);
      return str;
    }
    free(r);
  }
  return NULL;
}
