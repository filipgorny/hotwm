#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "gui.h"
#include "window.h"
#include "client.h"
#include "style.h"
#include <xcb/xproto.h>
#include <xcb/xcb.h>

#define WINDOW_PADDING 2
#define TITLE_BAR_HEIGHT 20


typedef struct {
    xcb_connection_t *conn;
    xcb_screen_t *screen;
    xcb_window_t root;
} Decorator;

Decorator *decorator_initialize(xcb_connection_t *conn, xcb_screen_t *screen);
void decorator_decoration_init(Decorator *decorator, Window *window);
void decorator_decorate_window(Decorator *decorator, Window *window, Style *style);
void decorator_refresh(Decorator *decorator, Client *clients, Style *style);

#ifdef __cplusplus
}
#endif
