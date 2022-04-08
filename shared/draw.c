#include "draw.h"
#include "arg.h"
#include <bits/stdint-uintn.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

Draw *draw_init(xcb_connection_t *conn, xcb_screen_t *screen,
                xcb_drawable_t *root) {
  Draw *draw = (Draw *)malloc(sizeof(Draw));
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

  xcb_rectangle_t rect = {(int16_t)x, (int16_t)y, (uint16_t)w, (uint16_t)h};
  xcb_poly_fill_rectangle(draw->conn, window, draw->gc, 1, &rect);
  xcb_flush(draw->conn);
}

void draw_text(Draw *draw, xcb_window_t window, int x, int y, const char *text,
               const uint32_t color, const uint32_t background_color,
               char *font) {
  uint32_t values[] = {color, background_color};

  xcb_change_gc(draw->conn, draw->gc, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND,
                values);

  xcb_font_t font_gc = xcb_generate_id(draw->conn);
  xcb_open_font(draw->conn, font_gc, strlen(font), font);

  uint32_t values_fonts[] = {font_gc};

  xcb_change_gc(draw->conn, draw->gc, XCB_GC_FONT, values_fonts);

  xcb_char2b_t *r = NULL;
  r = (xcb_char2b_t *)malloc(sizeof(xcb_char2b_t) * strlen(text));
  for (int i = 0; i < strlen(text); i++) {
    r[i].byte1 = 0;
    r[i].byte2 = text[i];
  }

  int height = 0;

  xcb_query_text_extents_reply_t *reply = xcb_query_text_extents_reply(
      draw->conn, xcb_query_text_extents(draw->conn, font_gc, 1, r), NULL);

  if (reply) {

    height = reply->font_ascent + reply->font_descent;
  } else {
    height = 20;
  }
  xcb_image_text_16(draw->conn, strlen(text), window, draw->gc, x, height + y,
                    r);

  xcb_flush(draw->conn);
  xcb_close_font(draw->conn, font_gc);
}
