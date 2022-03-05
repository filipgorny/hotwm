#include "grid.h"
#include "input.h"
#include "screen.h"
#include "session.h"
#include "spawn.h"
#include "window.h"
#include <stdio.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

Session *session;
InputConfig *input_config;

xcb_connection_t *conn;
const xcb_setup_t *setup;
xcb_drawable_t root;
xcb_screen_t *screen;

void handle_map_request(xcb_window_t window) {
  Window *w = window_create(&window);
  Client *c = client_create(w);

  session_add_client(session, c);

  int values[3];

  values[0] =
      XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
      XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE;
  xcb_change_window_attributes_checked(conn, window, XCB_CW_EVENT_MASK, values);

  xcb_map_window(conn, window);

  screen_refresh();
}

void handle_key_press(xcb_key_press_event_t *event) {
  input_handle_key_event(input_config, event);
}

void handle_button_press(xcb_button_press_event_t *ev) {
  /*  if (ev->detail == XCB_BUTTON_INDEX_1) {
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
    }*/
}

void handle_button_release(xcb_button_release_event_t *ev) {
  /*  Client *c = session->current_monitor->current_desktop->selected_client;

    if (c) {
      if (mouse_get_state() == MOUSE_STATE_BUTTON1_DOWN_META) {
        if (c->is_floating) {
          c->x = c->x - c->display_offset_x;
          c->y = c->y - c->display_offset_y;
        }
      }
    }

    mouse_update_state(MOUSE_STATE_FREE);
  */
}

void handle_mouse_motion(xcb_motion_notify_event_t *event) {
  /*  mouse_update_cords(event->root_x, event->root_y);

    Client *c = session->current_monitor->current_desktop->selected_client;

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
    }*/
}

void configure() {
  Arg arg = {.v = "/bin/st"};
  input_define_key(input_config, KEY_ENTER, MODKEY, spawn, &arg);
}

int main() {
  conn = xcb_connect(NULL, NULL);

  session = session_start();
  input_config = input_create_config(conn);

  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;
  xcb_flush(conn);

  root = screen->root;

  configure();

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

  xcb_generic_event_t *ev;

  while (1) {
    xcb_flush(conn);
    ev = xcb_wait_for_event(conn);

    switch (ev->response_type) {

    case XCB_EVENT_MASK_PROPERTY_CHANGE:
      printf("[Event] Property changed\n");
      xcb_property_notify_event_t *property_event =
          (xcb_property_notify_event_t *)ev;
      /*
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
                    Client *c =
         session->current_monitor->current_desktop->clients;

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
            }*/
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

  input_config_free(input_config);

  return 0;
}
