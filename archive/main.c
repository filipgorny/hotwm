#include <X11/Xutil.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include "debug.h"

#include "manager.h"

#include "config.h"

#include "session.h"

const xcb_setup_t *setup;
xcb_generic_event_t *ev;

xcb_drawable_t root;

static unsigned int numlockmask = 0;

static void handle_map_request(xcb_window_t window) {
  xcb_map_window(conn, window);

  manager_create_window(manager_session, window);

  xcb_flush(conn);
}

static void handle_key_press(xcb_key_press_event_t *event) {
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
  xcb_keysym_t keysym;
  keysym =
      (!(keysyms) ? 0 : xcb_key_symbols_get_keysym(keysyms, event->detail, 0));
  xcb_key_symbols_free(keysyms);

  int i;

  manager_trigger_key(manager_session, keysym, event->state);
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

  manager_session =
      manager_start_session(conn, screen, manager_setting_create());

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
