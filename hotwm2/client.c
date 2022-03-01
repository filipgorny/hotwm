#include <stdlib.h>
#include <sys/types.h>
#include <xcb/xcb.h>

#include "client.h"

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

Client *client_put_on_top(Client *client, Client *target) {
  int cx = client->x;
  int cy = client->y;

  client->x = target->x;
  client->y = target->y;

  target->x = cx;
  target->y = cy;

  if (target != NULL && client == target) {
    return client;
  }

  Client *next = target->next;

  target->next = client;

  Client *c = client;

  while (c->next != NULL) {
    c = c->next;

    if (c->next == target) {
      c->next = next;
    }
  }

  return target;
}

int client_count(Client *client) {
  Client *c = client;

  int count = 0;

  while (c != NULL) {
    count++;
    c = c->next;
  }

  return count;
}

Client *client_find_parent(Client *client, Client *child) {
  Client *c = client;

  while (c) {
    if (c->next == child) {
      return c;
    }

    c = c->next;
  }
}
