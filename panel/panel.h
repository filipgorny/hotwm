#pragma once

#define PANEL_HEIGHT 64

typedef struct {

} Panel;

Panel *panel_create(xcb_connection_t conn, xcb_screen_t screen, xcb_window_t root, int x, int y, int w, int h);
