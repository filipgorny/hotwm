#include "mouse.h"
#include <stdlib.h>

Mouse *mouse;

void mouse_init() { mouse = malloc(sizeof(Mouse)); }

void mouse_update_cords(int x, int y) {
  mouse->x = x;
  mouse->y = y;
}

void mouse_update_state(enum mouse_state state) { mouse->state = state; }

enum mouse_state mouse_get_state() { return mouse->state; }

void mouse_update_press_cords(x, y) {
  mouse->press_x = x;
  mouse->press_y = y;
}

int mouse_get_press_cords_x() { return mouse->press_x; }
int mouse_get_press_cords_y() { return mouse->press_y; }
