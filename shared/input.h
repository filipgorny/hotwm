#pragma once

#include <xcb/xcb_keysyms.h>

#define CALLBACK (*func)(Arg *arg);
#define MODKEY XCB_MOD_MASK_1

#define KEY_ENTER 0xff0d

typedef struct {
  int i;
  char *v;
} Arg;

typedef struct {
  unsigned int modifiers;
  xcb_keysym_t keysym;
  void CALLBACK
    Arg *arg;
} Key;

typedef struct {
    int key_index;
    xcb_connection_t *conn;
    Key *keys[];
} InputConfig;

InputConfig *input_create_config(xcb_connection_t *conn);
void input_define_key(InputConfig *config, xcb_keysym_t kesym, unsigned int modifiers, void (*func)(Arg*), Arg *arg);
void input_handle_key_event(InputConfig *config, xcb_key_press_event_t *event);
void input_config_free(InputConfig *config);
