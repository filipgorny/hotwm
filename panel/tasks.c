#include "tasks.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xproto.h>

#define EWMH_ENABLED 1

Task *tasks_get_all(xcb_connection_t *conn, xcb_window_t root,
                    xcb_ewmh_connection_t *ewmh, int screen_number) {
  xcb_get_property_reply_t *reply;
  xcb_get_property_cookie_t cookie;
  xcb_window_t *windows;
  cookie = xcb_ewmh_get_client_list_unchecked(ewmh, screen_number);
  xcb_ewmh_get_windows_reply_t winlist;

  xcb_window_t *windows_list;
  int windows_count;

  if (EWMH_ENABLED &&
      xcb_ewmh_get_client_list_reply(ewmh, cookie, &winlist, NULL)) {
    windows_list =
        (xcb_window_t *)malloc(sizeof(xcb_window_t) * winlist.windows_len);
    windows_count = winlist.windows_len;

    for (int i = 0; i < winlist.windows_len; i++) {
      windows_list[i] = winlist.windows[i];
    }
  } else {
    windows_count = 0;
    xcb_intern_atom_cookie_t ac = xcb_intern_atom(
        conn, 1, strlen("_NET_CLIENT_LIST"), "_NET_CLIENT_LIST");

    const xcb_intern_atom_reply_t *r;
    r = xcb_intern_atom_reply(conn, ac, NULL);

    if (r == NULL || r->atom == XCB_NONE) {
      log_error("Could not get client list");

      return NULL;
    }

    xcb_get_property_cookie_t cookie = xcb_get_property(
        conn, 0, root, r->atom, XCB_ATOM_WINDOW, 0, UINT32_MAX);

    reply = xcb_get_property_reply(conn, cookie, NULL);

    if (reply == NULL) {
      log_error("Could not get client list");

      return NULL;
    }

    xcb_window_t *value = (xcb_window_t *)xcb_get_property_value(reply);
    int value_size = xcb_get_property_value_length(reply);
    windows_count = value_size / sizeof(xcb_window_t);

    windows_list = (xcb_window_t *)malloc(sizeof(xcb_window_t) * windows_count);

    for (int i = 0; i < windows_count; i++) {
      windows_list[i] = value[i];
    }
  }

  Task *prev_task = NULL;

  Task *tasks = NULL;
  xcb_ewmh_get_utf8_strings_reply_t ewmh_txt_prop;

  for (int i = 0; i < windows_count; i++) {
    Task *task = (Task *)malloc(sizeof(Task));
    task->next = NULL;
    task->name = "";
    task->window = windows_list[i];

    cookie = xcb_ewmh_get_wm_visible_name_unchecked(ewmh, windows_list[i]);

    xcb_ewmh_get_wm_name_reply(ewmh,
                               xcb_ewmh_get_wm_name(ewmh, windows_list[i]),
                               &ewmh_txt_prop, NULL);

    task->name = ewmh_txt_prop.strings;

    if (sizeof(task->name) == 0) {
      task->name = "";
    }

    ewmh_txt_prop.strings = NULL;

    if (tasks == NULL) {
      tasks = task;
    }

    if (prev_task != NULL) {
      prev_task->next = task;
    }

    prev_task = task;
  }

  return tasks;
}
