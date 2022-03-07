#include "session.h"
#include <stdlib.h>

Session *session_start(xcb_screen_t *screen) {
  Session *session = malloc(sizeof(Session));

  Desktop *desktop = malloc(sizeof(Desktop));
  desktop->screen = screen;
  desktop->current_client = NULL;
  desktop->clients = NULL;

  session->desktops = desktop;
  session->current_desktop = desktop;

  session->current_layout_index = 0;

  return session;
}

void session_add_client(Session *session, Client *client) {
  Client *c = session->current_desktop->clients;

  if (c == NULL) {
    session->current_desktop->clients = client;
    session->current_desktop->clients->next = NULL;

    return;
  }

  while (c) {

    if (c->next == NULL) {
      c->next = client;
      break;
    }

    c = c->next;
  }
}

void session_add_layout(Session *session, Layout *layout) {
  session->layouts[session->layout_count] = *layout;

  session->current_layout_index = session->layout_count;

  session->layout_count++;
}

Layout *session_current_layout(Session *session) {
  return &session->layouts[session->current_layout_index];
}
