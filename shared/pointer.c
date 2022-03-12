#include "pointer.h"

#include <stdlib.h>
#include <xcb/xproto.h>

Pointer *pointer_initialize() {
  Pointer *p = malloc(sizeof(Pointer));

  p->last_action = malloc(sizeof(PointerAction));

  return p;
}

void pointer_update_cords(Pointer *pointer, int x, int y) {
  pointer->x = x;
  pointer->y = y;
}

void pointer_update_action(Pointer *pointer, bool button1, bool button2,
                           bool button3, xcb_mod_mask_t modmask,
                           Window *window) {
  pointer->last_action->button1 = button1;
  pointer->last_action->button2 = button2;
  pointer->last_action->button3 = button3;
  pointer->last_action->modmask = modmask;
  pointer->last_action->window = window;
}
