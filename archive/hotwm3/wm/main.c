#include "controller.h"
#include "decorator.h"
#include "grid.h"
#include "gui.h"
#include "keymap.h"
#include "log.h"
#include "manager.h"
#include "scripting.h"
#include "session.h"
#include "spawn.h"
#include "style.h"
#include "window.h"

#include <stdio.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include <X11/keysym.h>

#include "layout_mono.h"
#include "layout_stack.h"

Session *session;
Gui *gui;
Decorator *decorator;
Style *style;
ScriptingContext *scripting_context;
Keymap *keymap;
Controller *controller;
Manager *manager;

xcb_connection_t *conn;
const xcb_setup_t *setup;
xcb_drawable_t root;
xcb_screen_t *screen;

void refresh() {
  printf("[Refresh] Current layout is '%s'\n",
         session_current_layout(session)->name);

  session_current_layout(session)->apply(
      session_current_layout(session), screen,
      session->current_desktop->current_client,
      session->current_desktop->current_client, style);

  decorator_refresh(decorator, session->current_desktop->clients, style);
}

void handle_screen_modify() {
  // TODO - doesn't work
  Client *c = session->current_desktop->clients;

  while (c) {
    decorator_decorate_window(decorator, c->window, style);

    c = c->next;
  }
}

void handle_map_request(xcb_window_t window) {
  int values[3];

  values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
              XCB_EVENT_MASK_STRUCTURE_NOTIFY |
              XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
              XCB_EVENT_MASK_PROPERTY_CHANGE | XCB_EVENT_MASK_KEY_PRESS,
  xcb_change_window_attributes_checked(conn, window, XCB_CW_EVENT_MASK, values);

  xcb_map_window(conn, window);

  Window *w = window_create(conn, screen, root, window);

  Client *c = client_create(w);

  session_add_client(session, c);
  printf("Before window type\n");
  int window_type = 1; // manager_detect_window_type(manager, w);

  session->current_desktop->current_client = c;

  c->name = w->title = window_find_name(w);

  if (window_type > 0 && window_type < 10) {
    decorator_decoration_init(decorator, w);
    decorator_decorate_window(decorator, w, style);
  }

  refresh();
}

void handle_key_press(xcb_key_press_event_t *event) {
  Keybind *binding = keymap_get_keybind(keymap, event->detail, event->state);

  if (binding != NULL) {
    controller_run(controller, binding->cmd, binding->arg);
  }

  if (session->current_desktop->current_client != NULL) {
    xcb_send_event(conn, 0,
                   session->current_desktop->current_client->window->subwindow,
                   XCB_EVENT_MASK_KEY_PRESS, (char *)event);
  }
}

void handle_button_press(xcb_button_press_event_t *ev) {
  if (ev->detail == XCB_BUTTON_INDEX_1 && ev->state == XCB_MOD_MASK_1) {
    Client *target_client =
        session_find_client_by_xcb_window(session, ev->child);

    if (target_client) {
      Window *target_window = target_client->window;

      if (target_window) {
        manager_start_dragging(manager, target_window,
                               ev->event_x - target_window->x,
                               ev->event_y - target_window->y);
      }
    }
  }
}

void handle_button_release(xcb_button_release_event_t *ev) {
  manager_stop_dragging(manager);
}

void handle_mouse_motion(xcb_motion_notify_event_t *event) {
  if (manager->dragging) {
    manager->dragging->x = event->event_x - manager->drag_offset_x;
    manager->dragging->y = event->event_y - manager->drag_offset_y;

    window_update(manager->dragging);
  }
}

void handle_property_change(xcb_property_notify_event_t *event) {
  if (event->state == XCB_PROPERTY_NEW_VALUE) {
    if (event->atom == XCB_ATOM_WM_NAME) {
      Client *c = session_find_client_by_xcb_window(session, event->window);

      c->name = c->window->title = window_find_name(c->window);
    }
  }
}

#include "modules/spawn_module.h"
#include "modules/style_module.h"

void configure() {
  session_add_layout(session, layout_create("mono", layout_mono_apply));
  session_add_layout(session, layout_create("stack", layout_stack_apply));

  session_select_layout(session, "mono");
  /*
    style->gap = 8;
    style->margin = 32;
    style->title_bar_color = 0x00111111f;
    style->title_bar_text_color = 0x00fffffff;
    style->window_border_color = 0x00333333f;
    style->title_bar_height = 32;
    style->window_padding = 1;
    style->title_bar_margin = 1;
    style->title_bar_text_padding_bottom = 6;
    style->title_bar_text_padding_left = 2;
  */

  // @TODO fix colors in scripts
  style_define(style, "title_bar_color", 0x00111111f);
  style_define(style, "title_bar_text_color", 0x00fffffff);
  style_define(style, "window_border_color", 0x00333333f);

  scripting_declare_binding(scripting_context, "style", style);
  scripting_register_module(scripting_context, module_style);
  scripting_register_module(scripting_context, module_spawn);

  scripting_run(scripting_context,
                "/home/filip/Projects/filipgorny/hotwm/cfg/wm/init.lua");

  log_info("Config", "Configuration loaded");
}

int main() {
  conn = xcb_connect(NULL, NULL);
  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;
  xcb_flush(conn);
  root = screen->root;

  session = session_start(screen);

  gui = gui_initialize(draw_init(conn, screen, &root));
  decorator = decorator_initialize(conn, screen);

  style = style_create();

  keymap = keymap_create();

  controller = controller_create();

  manager = manager_initialize();

  scripting_context = scripting_create_context(controller, keymap, session);
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

      handle_property_change(property_event);
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
      // printf("[Event] Mouse motion\n");
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
    case 22: // XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY:
      printf("[Event] Sub structure notify\n");
      handle_screen_modify();
      break;
    default:
      printf("[Event] Unknown, %d\n", ev->response_type);
      xcb_flush(conn);
      break;
    }
  }

  return 0;
}
