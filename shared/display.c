#include "display.h"
#include <stdlib.h>

#define GRID_RESOLUTION 16

Display *display_create(xcb_screen_t *screen) {
  Display *display = malloc(sizeof(Display));
  display->screen = screen;

  return display;
}
