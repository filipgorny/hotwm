#include "session.h"
#include <stdlib.h>

Session *session_start() {
  Session *session = malloc(sizeof(Session));

  Desktop *desktop = malloc(sizeof(Desktop));
  session->desktops = desktop;
  session->current_desktop = desktop;

  return session;
}

void session_add_client(Session *session, Client *client) {
  Client *c = session->current_desktop->clients;

  if (c == NULL) {
    session->current_desktop->clients = client;

    return;
  }

  while (c) {
    c = c->next;
  }

  c->next = client;
}
