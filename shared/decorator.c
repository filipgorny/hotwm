#include "decorator.h"
#include "draw.h"
#include "log.h"
#include "style.h"
#include "window.h"

#include <cstdint>
#include <stdlib.h>
#include <xcb/xcb.h>

Decorator *decorator_initialize(xcb_connection_t *conn, xcb_screen_t *screen) {
  Decorator *decorator = (Decorator *)malloc(sizeof(Decorator));
  decorator->conn = conn;
  decorator->screen = screen;
  decorator->root = screen->root;

  return decorator;
}

void decorator_decoration_init(Decorator *decorator, Window *window) {
  window->draw = draw_init(decorator->conn, decorator->screen, &window->window);
}

void decorator_decorate_window(Decorator *decorator, Window *window,
                               Style *style) {
  log_info("Decorator", "Drawing decoration of parent window");

  window_update(window);

  xcb_window_t win = window->window;

  // border
  draw_rect(window->draw, win, 0, 0, window->width, window->height,
            style_get(style, "window_border_color"));

  // title bar
  draw_rect(window->draw, win, style_get(style, "window_padding"),
            style_get(style, "window_padding"),
            window->width - style_get(style, "window_padding") * 2,
            style_get(style, "title_bar_height"),
            style_get(style, "title_bar_color"));

  // title bar text
  draw_text(window->draw, win,
            style_get(style, "window_padding") +
                style_get(style, "title_bar_text_padding_left"),
            style_get(style, "window_padding") +
                style_get(style, "title_bar_height") -
                style_get(style, "title_bar_text_padding_bottom"),
            window->title, style_get(style, "title_bar_text_color"),
            style_get(style, "title_bar_color"), "hack-24");

  int subwindow_x = style_get(style, "window_padding");
  int subwindow_y = style_get(style, "title_bar_height") +
                    style_get(style, "title_bar_margin") +
                    style_get(style, "window_padding");
  int subwindow_width = window->width - style_get(style, "window_padding") * 2;
  int subwindow_height = window->height -
                         style_get(style, "window_padding") * 2 -
                         style_get(style, "title_bar_height") -
                         style_get(style, "title_bar_margin");

  xcb_configure_window(
      window->conn, window->subwindow,
      XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
      (uint32_t[]){(uint32_t)subwindow_width, (uint32_t)subwindow_height});
  xcb_configure_window(
      window->conn, window->subwindow,
      XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
      (uint32_t[]){(uint32_t)subwindow_x, (uint32_t)subwindow_y});
}

void decorator_refresh(Decorator *decorator, Client *clients, Style *style) {
  Client *c = clients;

  while (c) {
    decorator_decorate_window(decorator, c->window, style);

    c = c->next;
  }
}
