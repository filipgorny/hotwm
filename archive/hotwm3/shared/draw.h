#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <bits/stdint-uintn.h>
#include <sys/types.h>
#include <xcb/xcb.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xcb.h>

typedef struct {
  xcb_connection_t *conn;
  xcb_screen_t *screen;
  cairo_surface_t *surface;
  cairo_t *cr;
  uint32_t gc;
} Draw;

Draw *draw_init(xcb_connection_t *conn, xcb_screen_t *screen, xcb_drawable_t *root);

void draw_rect(Draw *draw, xcb_window_t window, int x, int y, int w, int h, const uint32_t color);
void draw_text(Draw *draw, xcb_window_t window, int x, int y, const char *text, const uint32_t color, const uint32_t background_color, char *font);

#ifdef __cplusplus
}
#endif
