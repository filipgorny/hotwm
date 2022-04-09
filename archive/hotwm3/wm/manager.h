#pragma once

#include <sys/types.h>
#include <xcb/xcb_ewmh.h>

#include "window.h"

#define WINDOW_TYPE_UNKNOWN 0
#define WINDOW_TYPE_NORMAL 1 
#define WINDOW_TYPE_DIALOG 2
#define WINDOW_TYPE_NOTIFICATION 12
#define WINDOW_TYPE_TOOLTIP 16
#define WINDOW_TYPE_TOOLBAR 17
#define WINDOW_TYPE_UTILITY 18
#define WINDOW_TYPE_BAR 23
#define WINDOW_TYPE_DOCK 24
#define WINDOW_TYPE_DESKTOP 33
#define WINDOW_TYPE_MENU 41
#define WINDOW_TYPE_DROPDOWN_MENU 42
#define WINDOW_TYPE_POPUP_MENU 43
#define WINDOW_TYPE_SPLASH 51
#define WINDOW_TYPE_COMBO 91
#define WINDOW_TYPE_DND 92

typedef struct {
    xcb_connection_t *conn;
    xcb_ewmh_connection_t *ewmh;
	int drag_offset_x, drag_offset_y;
	Window* dragging;
} Manager;

Manager *manager_initialize();
int manager_detect_window_type(Manager *manager, Window *window);

void manager_start_dragging(Manager *manager, Window *window, int offset_x, int offset_y);
void manager_stop_dragging(Manager *manager);
void manager_drag(Manager *manager, int x, int y);
