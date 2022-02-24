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

void draw_init(xcb_connection_t *conn, xcb_screen_t *screen);

void draw_rect(xcb_window_t window, int x, int y, int w, int h, const uint32_t color);
