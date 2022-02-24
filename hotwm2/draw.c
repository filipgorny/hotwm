#include "draw.h"
#include <bits/stdint-uintn.h>
#include <xcb/xproto.h>

Draw *draw;

void draw_init(xcb_connection_t *conn, xcb_screen_t *screen) {
  draw = malloc(sizeof(Draw));
  draw->conn = conn;
  draw->screen = screen;
  draw->gc = xcb_generate_id(conn);
  xcb_create_gc(conn, draw->gc, screen->root, XCB_GC_FOREGROUND,
                (const uint32_t[]){0xff0000});
}

void draw_rect(xcb_window_t window, int x, int y, int w, int h,
               const uint32_t color) {

  uint32_t values[] = {color};
  xcb_change_gc(draw->conn, draw->gc, XCB_GC_FOREGROUND, values);

  xcb_rectangle_t rect = {x, y, w, h};
  xcb_poly_fill_rectangle(draw->conn, window, draw->gc, 1, &rect);
  //  xcb_flush(draw->conn);
}