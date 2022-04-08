#include "draw.h"
#include "gui.h"
#include "log.h"
#include "panel.h"
#include "style.h"
#include "tasks.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

xcb_connection_t *conn;
const xcb_setup_t *setup;
xcb_drawable_t root;
xcb_screen_t *screen;
xcb_ewmh_connection_t *ewmh;

Panel *panel;
Draw *draw;
Task *tasks;
Gui *gui;
Style *style;

void configure() { style_define(style, "task_button_width", 128); }

void update() {
  panel_update_tasks(panel, tasks_get_all(conn, root, ewmh, 0));
  panel_draw_gui(panel);
}

int main() {
  conn = xcb_connect(NULL, NULL);
  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;

  root = screen->root;
  ewmh = (xcb_ewmh_connection_t *)malloc(sizeof(xcb_ewmh_connection_t));
  if (xcb_ewmh_init_atoms_replies(ewmh, xcb_ewmh_init_atoms(conn, ewmh),
                                  NULL) == 0) {
    fprintf(stderr, "Could not init EWMH atoms\n");
    return 1;
  }

  draw = draw_init(conn, screen, &root);
  gui = gui_initialize(draw);
  style = style_create();

  panel =
      panel_create(conn, screen, root, screen->width_in_pixels - PANEL_HEIGHT,
                   screen->height_in_pixels - PANEL_HEIGHT,
                   screen->width_in_pixels, PANEL_HEIGHT, draw);

  configure();

  while (1) {
    update();

    xcb_flush(conn);

    usleep(100000);
  }
}
