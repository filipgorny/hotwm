#include "session.h"
#include <stdlib.h>
#include <string.h>

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
    session->current_desktop->clients->prev = NULL;

    return;
  }

  while (c) {

    if (c->next == NULL) {
      c->next = client;
      client->prev = c;

      break;
    }

    c = c->next;
  }
}

void session_add_layout(Session *session, Layout *layout) {
  session->layouts[session->layout_count] = layout;

  session->current_layout_index = session->layout_count;

  session->layout_count++;
}

Layout *session_current_layout(Session *session) {
  return session->layouts[session->current_layout_index];
}

Client *session_find_client_by_xcb_window(Session *session,
                                          xcb_window_t window) {
  Client *c = session->current_desktop->clients;

  while (c) {
    if (c->window->window == window || c->window->subwindow == window) {
      return c;
    }

    c = c->next;
  }

  return NULL;
}

void session_select_next_client(Session *session) {
  if (!session->current_desktop->current_client) {
    if (session->current_desktop->clients) {
      session->current_desktop->current_client =
          session->current_desktop->clients;
    }

    return;
  }

  if (session->current_desktop->current_client->next) {
    session->current_desktop->current_client =
        session->current_desktop->current_client->next;
  } else {
    // select first client

    session->current_desktop->current_client =
        session->current_desktop->clients;
  }
}

void session_select_previous_client(Session *session) {
  if (!session->current_desktop->current_client) {
    if (session->current_desktop->clients) {
      session->current_desktop->current_client =
          session->current_desktop->clients;
    }

    return;
  }

  if (session->current_desktop->current_client->prev) {
    session->current_desktop->current_client =
        session->current_desktop->current_client->prev;
  }
}

Layout *session_get_layout(Session *session, char *name) {
  int i;

  for (i = 0; i < session->layout_count; i++) {
    if (strcmp(session->layouts[i]->name, name) == 0) {
      return session->layouts[i];
    }
  }

  return NULL;
}

int session_desktop_client_count(Desktop *desktop) {
  Client *c = desktop->clients;

  int count = 0;

  while (c) {
    count++;

    c = c->next;
  }

  return count;
}

void session_select_layout(Session *session, char *name) {
  int i;

  for (i = 0; i < session->layout_count; i++) {
    if (strcmp(session->layouts[i]->name, name) == 0) {
      session->current_layout_index = i;

      return;
    }
  }
}
