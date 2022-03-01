#include "layout.h"
#include "client.h"

void layout_stacked() {
  Client *c = session->current_monitor->current_desktop->clients;

  int length = 0;

  do {
    if (c->is_open && c != session->current_client) {
      length++;
    }
  } while (c = c->next);

  int window_height = session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  int current_y = BAR_MARGIN + GAP_WIDTH;

  int i;
  if (length > 0) {
    session->current_client->x =
        STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw + GAP_WIDTH;

    session->current_client->width =
        session->current_monitor->mw - 2 * GAP_WIDTH -
        STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw;

  } else {
    session->current_client->x = GAP_WIDTH;

    session->current_client->width =
        session->current_monitor->mw - 2 * GAP_WIDTH;
  }

  session->current_client->y = BAR_MARGIN + GAP_WIDTH;
  session->current_client->height =
      session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  update_client_geometry(session->current_client);

  c = session->current_monitor->current_desktop->clients;

  while (c) {
    xcb_window_t target_window = c->parent;
    xcb_map_window(conn, target_window);

    if (!c->is_floating && c->is_open && c != session->current_client) {

      c->x = GAP_WIDTH;
      c->y = current_y;
      c->width = STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw -
                 GAP_WIDTH;

      c->height = (int)((session->current_monitor->mh - BAR_MARGIN -
                         (length + 1) * GAP_WIDTH) /
                        length);

      update_client_geometry(c);

      current_y += c->height + GAP_WIDTH;
    }

    c = c->next;
  }

  xcb_flush(conn);
}

void layout_mono() {
  Client *c = session->current_monitor->current_desktop->clients;

  while (c) {
    if (c->is_floating) {
      continue;
    }

    xcb_unmap_window(conn, c->parent);

    c = c->next;
  }

  xcb_map_window(conn, session->selected_client->parent);

  session->selected_client->x = GAP_WIDTH;
  session->selected_client->y = BAR_MARGIN + GAP_WIDTH;
  session->selected_client->width =
      session->current_monitor->mw - 2 * GAP_WIDTH;
  session->selected_client->height =
      session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  update_client_geometry(session->selected_client);
}
