#pragma once

#include "draw.h"
#include "gui.h"
#include "tasks.h"

#include <xcb/xcb.h>

#define PANEL_HEIGHT 64

typedef struct {
    int id;
    xcb_window_t window;
    Task *tasks;
    int tasks_count;
    GuiElement *gui_container_tasks;
    int height;
    int padding;
} Panel;

Panel *panel_create(xcb_connection_t *conn, xcb_screen_t *screen, xcb_window_t root, int x, int y, int w, int h, Draw *draw);

void panel_update_tasks(Panel *panel, Task *tasks);

void panel_draw_gui(Panel *panel);
