#include "manager.h"
#include "window.h"

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

Manager *manager_initialize(xcb_connection_t *conn) {
  Manager *manager = malloc(sizeof(Manager));
  manager->conn = conn;
  manager->ewmh = calloc(1, sizeof(xcb_ewmh_connection_t));
	manager->dragging = NULL;

  return manager;
}

void manager_start_dragging(Manager *manager, Window *window, int offset_x, int offset_y) {
	manager->dragging = window;
	manager->drag_offset_x = offset_x;
	manager->drag_offset_y = offset_y;
}

void manager_stop_dragging(Manager *manager) {
	manager->dragging = NULL;
}

void manager_drag(Manager *manager, int x, int y) {
	manager->dragging->x = x - manager->drag_offset_x;
	manager->dragging->y = y - manager->drag_offset_y;

	window_update(manager->dragging);
}

int manager_detect_window_type(Manager *manager, Window *window) {
  xcb_get_property_reply_t *reply = xcb_get_property_reply(
      manager->conn,
      xcb_get_property(manager->conn, 0, window->window,
                       manager->ewmh->_NET_WM_WINDOW_TYPE,
                       XCB_GET_PROPERTY_TYPE_ANY, 0, 1),
      NULL);

  if (reply == NULL) {
    return WINDOW_TYPE_UNKNOWN;
  }

  uint32_t *data = xcb_get_property_value(reply);
  uint32_t type = *data;
  free(reply);

  if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_DESKTOP) {
    return WINDOW_TYPE_DESKTOP;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_DOCK) {
    return WINDOW_TYPE_DOCK;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_TOOLBAR) {
    return WINDOW_TYPE_TOOLBAR;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_MENU) {
    return WINDOW_TYPE_MENU;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_UTILITY) {
    return WINDOW_TYPE_UTILITY;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_SPLASH) {
    return WINDOW_TYPE_SPLASH;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_DIALOG) {
    return WINDOW_TYPE_DIALOG;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_DROPDOWN_MENU) {
    return WINDOW_TYPE_DROPDOWN_MENU;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_POPUP_MENU) {
    return WINDOW_TYPE_POPUP_MENU;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_TOOLTIP) {
    return WINDOW_TYPE_TOOLTIP;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION) {
    return WINDOW_TYPE_NOTIFICATION;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_COMBO) {
    return WINDOW_TYPE_COMBO;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_DND) {
    return WINDOW_TYPE_DND;
  } else if (type == manager->ewmh->_NET_WM_WINDOW_TYPE_NORMAL) {
    return WINDOW_TYPE_NORMAL;
  } else {
    return WINDOW_TYPE_UNKNOWN;
  }
}
