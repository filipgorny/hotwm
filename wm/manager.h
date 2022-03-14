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

#define MANAGER_STATE_IDLE 0
#define MANAGER_STATE_RESIZING 1
#define MANAGER_STATE_MOVING 2

typedef struct {
    Window win;
    int x, y;
    int offset_x, offset_y;
    int width, height;
    int state_from, state_to;
} ManagerLastStateChange;

typedef struct {
    xcb_connection_t *conn;
    xcb_ewmh_connection_t *ewmh;
    int state;
    ManagerLastStateChange *last_state_change;
} Manager;

Manager *manager_initialize();
int manager_detect_window_type(Manager *manager, Window *window);
