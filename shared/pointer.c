#include "pointer.h"

#include <stdlib.h>

Pointer *pointer_initialize() {
  Pointer *p = malloc(sizeof(Pointer));

  PointerState *s = malloc(sizeof(PointerState));
  p->state = s;

  p->last_change = NULL;

  return p;
}

void pointer_update_cords(Pointer *pointer, int x, int y) {
  pointer->state->x = x;
  pointer->state->y = y;
}

void pointer_update_state(Pointer *pointer, bool button1, bool button2,
                          bool button3, bool meta) {
  if (pointer->last_change != NULL) {
    free(pointer->last_change);
  }

  PointerStateChange *state_change = malloc(sizeof(PointerStateChange));

  if (button1 && !button2 && !button3) {
    state_change->state =
        meta ? POINTER_STATE_BUTTON1_DOWN_META : POINTER_STATE_BUTTON1_DOWN;
  }

  if (!button1 && button2 && !button3) {
    state_change->state =
        meta ? POINTER_STATE_BUTTON2_DOWN_META : POINTER_STATE_BUTTON2_DOWN;
  }

  if (!button1 && !button2 && button3) {
    state_change->state =
        meta ? POINTER_STATE_BUTTON3_DOWN_META : POINTER_STATE_BUTTON3_DOWN;
  }

  state_change->x = pointer->state->x;
  state_change->y = pointer->state->y;

  pointer->last_change = state_change;
}
