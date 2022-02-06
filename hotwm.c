#include <X11/keysym.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include "debug.h"
#include "hot.h"

xcb_connection_t *conn;
xcb_screen_t *screen;
const xcb_setup_t *setup;
xcb_generic_event_t *ev;

xcb_drawable_t root;

static void handle_map_request(xcb_window_t window) {
  xcb_map_window(conn, window);

  int vals[4];

  vals[0] = 200;
  vals[1] = 200;
  vals[2] = screen->width_in_pixels / 2;
  vals[3] = screen->height_in_pixels / 2;
  vals[4] = 4;
  xcb_configure_window(conn, window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
                           XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       vals);

  vals[0] = 0xFFFFFF;
  xcb_change_window_attributes(conn, window, XCB_CW_BORDER_PIXEL, vals);

  xcb_flush(conn);
}

static void handle_key_press(xcb_key_press_event_t *event) {
  debug_info_int("Key pressed: %d", event->detail);

  if (event->detail == 36) {
    hot_run("/usr/bin/st");
  }

  xcb_flush(conn);
}

static void handle_button_release(xcb_generic_event_t *ev) {
  debug_info("Button released");
  xcb_flush(conn);
}

int main() {
  debug_info("Starting manager.");

  conn = xcb_connect(NULL, NULL);

  int ret = xcb_connection_has_error(conn);

  if (ret > 0) {
    debug_info_int("Connection error: %d", ret);
    exit(ret);
  }

  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;
  xcb_flush(conn);

  root = screen->root;

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

  while (1) {
    debug_info("Entering main loop\n");
    xcb_flush(conn);
    ev = xcb_wait_for_event(conn);
    switch (ev->response_type) {
    case 5: // Button release event
      handle_button_release(ev);
      break;
    case XCB_KEY_PRESS:
      debug_info("Keyboard Pressed\n");
      xcb_key_press_event_t *key_event = (xcb_key_press_event_t *)ev;
      handle_key_press(key_event);
      break;
    case XCB_MAP_REQUEST:
      debug_info("Map request\n");
      xcb_map_request_event_t *map_event = (xcb_map_request_event_t *)ev;
      handle_map_request(map_event->window);
      break;
    default:
      debug_info_int("Unknown event: %d\n", ev->response_type);
      xcb_flush(conn);
      break;
    }
  }

  return 0;
}
