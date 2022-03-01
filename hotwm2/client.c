#include <stdlib.h>
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
  printf("Lookinf for selected client\n");
  if (client == target) {
    return client;
  }

  Client *next = target->next;
  printf("AA1\n");

  target->next = client;

  Client *c = client;
  printf("AA2\n");
  while (c) {
    if (!c->next) {
      c->next = next;

      break;
    }

    c = c->next;
  }
  printf("AA3\n");

  target->next = client;

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
