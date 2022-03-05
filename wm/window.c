#include "window.h"
#include <stdlib.h>

Window *window_create(xcb_window_t *window) {
  Window *w = malloc(sizeof(Window));
  w->x = 0;
  w->y = 0;
  w->width = 0;
  w->height = 0;
  w->window = window;

  return w;
}
