#include <X11/X.h>
#include <X11/Xutil.h>
#include <cairo/cairo-xcb.h>
#include <cairo/cairo.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/randr.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include "main.h"

#include "config.h"

#include "gui.h"

#include "mouse.h"

const xcb_setup_t *setup;
xcb_generic_event_t *ev;

xcb_drawable_t root;

xcb_connection_t *conn;
xcb_screen_t *screen;

Session *session;

Session *session;

Session *init_session() {
  session = malloc(sizeof(Session));
  session->monitors = create_monitor(0);
  session->current_monitor = session->monitors;
  session->selected_client = NULL;
  session->main_client = NULL;
  session->mouse = malloc(sizeof(Mouse));

  return session;
}

xcb_window_t create_parent(Client *client) {
  xcb_window_t parent_window = xcb_generate_id(conn);
  xcb_create_window(conn, screen->root_depth, parent_window, root, 2, 20, 600,
                    500, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
                    0, NULL);

  // xcb_unmap_window(conn, client->window);
  xcb_reparent_window(conn, client->window, parent_window, WINDOW_PADDING,
                      TITLE_BAR_HEIGHT);

  xcb_map_window(conn, parent_window);

  xcb_flush(conn);

  return parent_window;
}

Desktop *create_desktop() {
  Desktop *desktop = malloc(sizeof(Desktop));
  desktop->clients = NULL;

  desktop->window = xcb_generate_id(conn);

  xcb_create_window(conn, screen->root_depth, desktop->window, root, 0, 0,
                    screen->width_in_pixels, screen->height_in_pixels, 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0,
                    NULL);

  xcb_map_window(conn, desktop->window);

  xcb_flush(conn);

  return desktop;
}

Monitor *create_monitor(int num) {
  Monitor *m = malloc(sizeof(Monitor));
  m->num = num;
  m->mx = 0;
  m->my = 0;
  m->mw = screen->width_in_pixels;
  m->mh = screen->height_in_pixels;
  m->next = NULL;
  m->desktops = create_desktop();
  m->current_desktop = m->desktops;
  m->layout = stacked;
  return m;
}

void spawn(const Arg *arg) {
  int status;

  if (fork() == 0) {
    status = system(arg->v);
    exit(0);
  }
}

Client *create_client(xcb_window_t window) {
  Client *c = malloc(sizeof(Client));
  c->window = window;
  c->next = NULL;
  c->is_floating = 0;
  c->is_maximized = 0;
  c->is_open = 1;
  c->parent = create_parent(c);
  c->is_selected = 0;
  c->conn = conn;

  c->index = session->current_index++;

  xcb_reparent_window(conn, c->parent,
                      session->current_monitor->current_desktop->window,
                      WINDOW_PADDING, TITLE_BAR_HEIGHT);

  return c;
}

void select_client(Client *client) {
  Client *c = session->current_monitor->current_desktop->clients;

  for (; c != NULL; c = c->next) {
    if (c == client) {
      c->is_selected = 1;
    } else {
      c->is_selected = 0;
    }
  }

  session->selected_client = client;
}

void active_down() {
  Client *c = session->current_monitor->current_desktop->clients;

  while (c->next) {
    if (c->next == session->selected_client) {
      select_client(c);

      redraw();

      return;
    }

    c = c->next;
  }
}

void active_up() {
  Client *c = session->current_monitor->current_desktop->clients;

  if (!c->next) {
    select_client(c);

    redraw();

    return;
  }

  while (c->next) {
    if (c == session->selected_client) {
      select_client(c->next);

      redraw();

      return;
    }

    c = c->next;
  }
}

void set_layout(const Arg *arg) {
  session->current_monitor->layout = arg->layout;
  redraw();
}

void toggle() {
  session->main_client = session->selected_client;
  redraw();

  return;
  Client *c = session->current_monitor->current_desktop->clients;

  while (c->next) {
    if (c == session->selected_client) {
      Client *current = session->main_client;

      session->main_client = c;
      c = current;
      c->next = current->next;

      redraw();

      return;
    }

    c = c->next;
  }
}

void handle_map_request(xcb_window_t window) {
  xcb_map_window(conn, window);

  Client *main_client = create_client(window);

  if (session->current_monitor->current_desktop->clients != NULL) {
    Client *c = session->current_monitor->current_desktop->clients;
    while (c->next) {
      c = c->next;
    }
    c->next = main_client;
  } else {
    session->current_monitor->current_desktop->clients = main_client;
  }

  int values[3];

  values[0] =
      XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
      XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE;
  xcb_change_window_attributes_checked(conn, window, XCB_CW_EVENT_MASK, values);

  session->main_client = main_client;

  select_client(main_client);

  redraw();
}

void handle_key_press(xcb_key_press_event_t *event) {
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
  xcb_keysym_t keysym;
  keysym =
      (!(keysyms) ? 0 : xcb_key_symbols_get_keysym(keysyms, event->detail, 0));
  xcb_key_symbols_free(keysyms);

  int i;

  for (i = 0; i < LENGTH(KEYS); i++) {
    if (keysym == KEYS[i].keysym &&
        CLEANMASK(event->state) == CLEANMASK(KEYS[i].modifiers)) {
      KEYS[i].func(&(KEYS[i].arg));

      xcb_flush(conn);

      return;
    }
  }

  if (session->main_client) {
    xcb_send_event(conn, 0, session->selected_client->window,
                   XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *)event);
  }

  xcb_flush(conn);
}

void handle_button_press(xcb_button_press_event_t *ev) {
  if (ev->detail == XCB_BUTTON_INDEX_1) {
    mouse_update_state(CLEANMASK(ev->state) == MODKEY
                           ? MOUSE_STATE_BUTTON1_DOWN_META
                           : MOUSE_STATE_BUTTON1_DOWN);
  } else if (ev->detail == XCB_BUTTON_INDEX_2) {
    mouse_update_state(CLEANMASK(ev->state) == MODKEY
                           ? MOUSE_STATE_BUTTON2_DOWN_META
                           : MOUSE_STATE_BUTTON2_DOWN);

  } else if (ev->detail == XCB_BUTTON_INDEX_3) {
    mouse_update_state(CLEANMASK(ev->state) == MODKEY
                           ? MOUSE_STATE_BUTTON3_DOWN_META
                           : MOUSE_STATE_BUTTON3_DOWN);
  }

  mouse_update_press_cords(ev->event_x, ev->event_y);

  xcb_window_t window = ev->event;
  Client *c = session_get_client_by_cords(ev->event_x, ev->event_y);

  if (c) {
    session_raise_client(c);
  }
}

void handle_button_release(xcb_button_release_event_t *ev) {
  Client *c = session->selected_client;

  if (c) {
    if (mouse_get_state() == MOUSE_STATE_BUTTON1_DOWN_META) {
      if (c->is_floating) {
        c->x = c->x - c->display_offset_x;
        c->y = c->y - c->display_offset_y;
      }
    }
  }

  mouse_update_state(MOUSE_STATE_FREE);
}

void handle_mouse_motion(xcb_motion_notify_event_t *event) {
  mouse_update_cords(event->root_x, event->root_y);

  Client *c = session->selected_client;

  if (c) {
    if (mouse_get_state() == MOUSE_STATE_BUTTON1_DOWN_META) {
      if (!c->is_floating) {
        c->is_floating = 1;
      }

      c->display_offset_x = mouse_get_press_cords_x() - event->root_x;
      c->display_offset_y = mouse_get_press_cords_y() - event->root_y;

      update_client_geometry(c);
      redraw();
    }
  }
}

void update_client_geometry(Client *c) {
  int vals[5];

  xcb_window_t target_window = c->parent;

  vals[0] = c->x - c->display_offset_x;
  vals[1] = c->y - c->display_offset_y;
  vals[2] = c->width;
  vals[3] = c->height;
  vals[4] = 0;
  xcb_configure_window(conn, target_window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
                           XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       vals);

  target_window = c->window;

  vals[0] = WINDOW_PADDING;
  vals[1] = TITLE_BAR_HEIGHT;
  vals[2] = c->width - WINDOW_PADDING * 2;
  vals[3] = c->height - TITLE_BAR_HEIGHT - WINDOW_PADDING;
  vals[4] = 0;
  xcb_configure_window(conn, target_window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
                           XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       vals);

  if (c == session->selected_client) {
    vals[0] = BORDER_COLOR_ACTIVE;
  } else {
    vals[0] = BORDER_COLOR_INACTIVE;
  }

  target_window = c->parent;

  xcb_change_window_attributes(conn, target_window, XCB_CW_BORDER_PIXEL, vals);

  gui_decorate_client(c);

  xcb_flush(conn);
}

void layout_stacked() {
  Client *c = session->current_monitor->current_desktop->clients;

  if (c == NULL) {
    return;
  }

  int length = 0;

  do {
    if (c->is_open && c != session->main_client) {
      length++;
    }
  } while (c = c->next);

  int window_height = session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  int current_y = GAP_WIDTH;

  int i;
  if (length > 0) {
    session->main_client->x =
        STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw + GAP_WIDTH;

    session->main_client->width =
        session->current_monitor->mw - 2 * GAP_WIDTH -
        STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw;

  } else {
    session->main_client->x = GAP_WIDTH;

    session->main_client->width = session->current_monitor->mw - 2 * GAP_WIDTH;
  }

  session->main_client->y = GAP_WIDTH;
  session->main_client->height =
      session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  update_client_geometry(session->main_client);

  c = session->current_monitor->current_desktop->clients;

  while (c) {
    xcb_window_t target_window = c->parent;
    xcb_map_window(conn, target_window);

    if (!c->is_floating && c->is_open && c != session->main_client) {

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

void layout_floating() {
  Client *c = session->current_monitor->current_desktop->clients;

  while (c) {
    if (!c->floating_initialized) {
      c->floating_x = c->x;
      c->floating_y = c->y;
      c->floating_width = c->width;
      c->floating_height = c->height;

      c->floating_initialized = 1;
    }

    c->x = c->floating_x;
    c->y = c->floating_y;
    c->width = c->floating_width;
    c->height = c->floating_height;

    update_client_geometry(c);

    c = c->next;
  }

  xcb_flush(conn);
}

void redraw() {
  if (session->current_monitor->layout == mono) {
    layout_mono();
  } else if (session->current_monitor->layout == stacked) {
    layout_stacked();
  } else if (session->current_monitor->layout == floating) {
    layout_floating();
  }

  draw_rect(session->current_monitor->current_desktop->window, 0, 0,
            screen->width_in_pixels, screen->height_in_pixels, COLOR_DESKTOP);

  xcb_flush(conn);
}

void toggle_floating(const Arg *arg) {
  if (session->selected_client->is_floating) {
    session->selected_client->is_floating = 0;
  } else {
    session->selected_client->is_floating = 1;
  }

  redraw();

  xcb_flush(conn);
}

Client *session_get_client_by_window(xcb_window_t window) {
  Client *c = session->current_monitor->current_desktop->clients;

  while (c) {
    if (c->parent == window && c->window == window) {
      return c;
    }

    c = c->next;
  }

  return NULL;
}

Client *session_get_client_by_cords(int x, int y) {
  Client *c = session->current_monitor->current_desktop->clients;

  while (c) {
    if (c->is_open && c->x <= x && c->x + c->width >= x && c->y <= y &&
        c->y + c->height >= y) {
      return c;
    }

    c = c->next;
  }

  return NULL;
}

void session_select_client(Client *client) {
  session->selected_client = client;
}

void session_raise_client(Client *client) {
  session_select_client(client);

  xcb_circulate_window(client->conn, XCB_CIRCULATE_LOWER_HIGHEST,
                       client->parent);

  session->current_monitor->current_desktop->clients = client_put_on_top(
      session->current_monitor->current_desktop->clients, client);

  redraw();
}

void desktop_next() {
  Desktop *next_desktop = session->current_monitor->current_desktop->next;

  if (!next_desktop) {
    next_desktop = create_desktop();
    session->current_monitor->current_desktop->next = next_desktop;

    xcb_unmap_window(conn, session->current_monitor->current_desktop->window);

    session->current_monitor->current_desktop = next_desktop;

    xcb_map_window(conn, next_desktop->window);
  }

  // redraw();

  printf("Desktop next\n");
}

int main() {
  conn = xcb_connect(NULL, NULL);

  int ret = xcb_connection_has_error(conn);

  if (ret > 0) {
    exit(ret);
  }

  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;
  xcb_flush(conn);

  root = screen->root;

  session = init_session();

  draw_init(conn, screen);
  mouse_init();

  redraw();

  int values[3];

  values[0] =
      XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
      XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE;
  xcb_change_window_attributes_checked(conn, root, XCB_CW_EVENT_MASK, values);

  xcb_grab_key(conn, 1, root, XCB_MOD_MASK_ANY, XCB_GRAB_ANY,
               XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
  xcb_grab_button(conn, 1, root,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, root, XCB_NONE,
                  XCB_BUTTON_INDEX_1, XCB_MOD_MASK_ANY);

  xcb_grab_pointer(conn, 1, root,
                   XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_PRESS |
                       XCB_EVENT_MASK_BUTTON_RELEASE,
                   XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, XCB_NONE, XCB_NONE,
                   XCB_CURRENT_TIME);
  while (1) {
    xcb_flush(conn);
    ev = xcb_wait_for_event(conn);

    switch (ev->response_type) {

    case XCB_EVENT_MASK_PROPERTY_CHANGE:
      printf("[Event] Property changed\n");
      xcb_property_notify_event_t *property_event =
          (xcb_property_notify_event_t *)ev;

      if (property_event->state == XCB_PROPERTY_NEW_VALUE) {
        if (property_event->atom == XCB_ATOM_WM_NAME) {
          xcb_get_property_cookie_t cookie = xcb_get_property(
              conn, 0, property_event->window, XCB_ATOM_WM_NAME,
              XCB_ATOM_STRING, 0, UINT32_MAX);

          xcb_get_property_reply_t *reply =
              xcb_get_property_reply(conn, cookie, NULL);

          if (reply) {
            char *name = xcb_get_property_value(reply);
            if (name) {
              Client *c = session->current_monitor->current_desktop->clients;

              while (c) {
                if (c->parent == property_event->window) {
                  c->name = name;
                  break;
                }

                c = c->next;
              }
            }
          }
        }
      }
      break;
    case 5: // XCB_EVENT_MASK_BUTTON_RELEASE: // Button release event
      printf("[Event] Button release\n");
      xcb_button_release_event_t *button_release_event =
          (xcb_button_release_event_t *)ev;

      handle_button_release(button_release_event);
      break;
    case XCB_EVENT_MASK_BUTTON_PRESS: // Button press event
      printf("[Event] Button press\n");
      xcb_button_press_event_t *button_press_event =
          (xcb_button_press_event_t *)ev;

      handle_button_press(button_press_event);
      break;
    case 6: // MOUSE MOTION
      printf("[Event] Mouse motion\n");
      xcb_motion_notify_event_t *motion_event = (xcb_motion_notify_event_t *)ev;

      handle_mouse_motion(motion_event);
      break;
    case XCB_KEY_PRESS:
      printf("[Event] Key press\n");
      xcb_key_press_event_t *key_event = (xcb_key_press_event_t *)ev;
      handle_key_press(key_event);
      break;
    case XCB_MAP_REQUEST:
      printf("[Event] Map request\n");
      xcb_map_request_event_t *map_event = (xcb_map_request_event_t *)ev;
      handle_map_request(map_event->window);
      break;
    default:
      printf("[Event] Unknown, %d\n", ev->response_type);
      xcb_flush(conn);
      break;
    }
  }

  return 0;
}
