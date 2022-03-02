#ifndef MOUSE_H
#define MOUSE_H

enum mouse_state {
    MOUSE_STATE_FREE,
    MOUSE_STATE_BUTTON1_DOWN,
    MOUSE_STATE_BUTTON2_DOWN,
    MOUSE_STATE_BUTTON3_DOWN,
    MOUSE_STATE_BUTTON1_DOWN_META,
    MOUSE_STATE_BUTTON2_DOWN_META,
    MOUSE_STATE_BUTTON3_DOWN_META,
};


typedef struct {
  int x, y;
  enum mouse_state state;
  int press_x, press_y;
} Mouse;

void mouse_init();
void mouse_update_cords(int x, int y);
void mouse_update_state(enum mouse_state state);
enum mouse_state mouse_get_state();
void mouse_update_press_cords(int x, int y);
int mouse_get_press_cords_x();
int mouse_get_press_cords_y();

#endif
