#include "draw.h"
#include <bits/stdint-uintn.h>
#include <stdlib.h>
#include <xcb/xproto.h>

Draw *draw_init(xcb_connection_t *conn, xcb_screen_t *screen,
                xcb_drawable_t *root) {
  Draw *draw = malloc(sizeof(Draw));
  draw->conn = conn;
  draw->screen = screen;
  draw->gc = xcb_generate_id(conn);

  xcb_create_gc(conn, draw->gc, *root, XCB_GC_FOREGROUND,
                (const uint32_t[]){0xff0000});

  return draw;
}

void draw_rect(Draw *draw, xcb_window_t window, int x, int y, int w, int h,
               const uint32_t color) {

  uint32_t values[] = {color};
  xcb_change_gc(draw->conn, draw->gc, XCB_GC_FOREGROUND, values);

  xcb_rectangle_t rect = {x, y, w, h};
  xcb_poly_fill_rectangle(draw->conn, window, draw->gc, 1, &rect);
  xcb_flush(draw->conn);
}

void draw_text(Draw *draw, xcb_window_t window, int x, int y, const char *text,
               const uint32_t color, const uint32_t background_color,
               char *font) {
  uint32_t values[] = {color, background_color};

  xcb_change_gc(draw->conn, draw->gc, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND,
                values);

  uint32_t values_fonts[] = {font};
  xcb_image_text_8(draw->conn, strlen(text), window, draw->gc, x, y, text);
  xcb_flush(draw->conn);
}
