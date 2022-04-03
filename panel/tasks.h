#pragma once

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

typedef struct Task Task;
struct Task {
    xcb_window_t window;
	char* name;
    Task* next;
};;

Task *tasks_get_all(xcb_connection_t *conn, xcb_window_t root, xcb_ewmh_connection_t *ewmh, int screen_number);
