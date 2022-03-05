#pragma once

#include <xcb/xcb_keysyms.h>

#define CALLBACK (*func)(const Arg *);

typedef union {
  int i;
  unsigned int ui;
  float f;
  char *v;
} Arg;

typedef struct {
  unsigned int modifiers;
  xcb_keysym_t keysym;
  void CALLBACK
  Arg arg;
} Key;

typedef struct {
    int key_index;
    xcb_key_symbols_t *keysyms;
    Key keys[];
} InputConfig;

InputConfig *input_create_config(xcb_connection_t *conn);
void input_define_key(InputConfig *config, xcb_keysym_t kesym, unsigned int modifiers, void (*func)(const Arg *), const Arg arg);
void input_handle_key_event(InputConfig *config, xcb_key_press_event_t *event);
void input_config_free(InputConfig *config);
