#include "pointer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

Pointer *pointer_initialize() {
  Pointer *p = malloc(sizeof(Pointer));

  p->update = malloc(sizeof(PointerUpdate));

  p->update->button1 = false;
  p->update->button2 = false;
  p->update->button3 = false;
  p->update->modmask = 0;
  p->update->x = 0;
  p->update->y = 0;

  p->prev_update = *&p->update;

  return p;
}

void pointer_update_cords(Pointer *pointer, int x, int y) {
  pointer->x = x;
  pointer->y = y;
}

void pointer_update_action(Pointer *pointer, bool button1, bool button2,
                           bool button3, xcb_mod_mask_t modmask, Window *window,
                           bool release) {
  pointer->prev_update = pointer->update;

  pointer->update->button1 = button1;
  pointer->update->button2 = button2;
  pointer->update->button3 = button3;
  pointer->update->modmask = modmask;
  pointer->update->window = window;
  pointer->update->release = release;
}

PointerUpdateBind *pointer_get_action_bind(Pointer *pointer, bool button1,
                                           bool button2, bool button3,
                                           xcb_mod_mask_t modmask,
                                           bool release) {
  PointerUpdateBind *p = pointer->action_binds;

  while (p) {
    if (p->button1 == button1 && p->button2 == button2 &&
        p->button3 == button3 && p->modmask == modmask &&
        p->release == release) {
      p->arg.x = pointer->x;
      p->arg.y = pointer->y;

      return p;
    }
    p = p->next;
  }

  return NULL;
}

void pointer_map_action(Pointer *pointer, char *action_desc, char *cmd,
                        Arg arg) {

  PointerUpdateBind *a = malloc(sizeof(PointerUpdateBind));
  a->button1 = false;
  a->button2 = false;
  a->button3 = false;
  a->modmask = 0;

  bool map_button_part = false;
  char *button_part;
  xcb_mod_mask_t modkey = 0;

  int i, ii = 0;

  for (i = 0; i < strlen(action_desc); i++) {
    if (action_desc[i] == '-') {
      map_button_part = true;
      button_part = malloc(strlen(action_desc) - i + 1);

      for (ii = i; ii < strlen(action_desc); ii++) {
        button_part[ii - i] = action_desc[ii];
      }

      button_part[ii - i] = '\0';
    }

    if (action_desc[i] == 'S') {
      modkey = XCB_MOD_MASK_1;
    }
  }

  char *check_button;

  if (map_button_part) {
    check_button = button_part;
  } else {
    check_button = action_desc;
  }

  for (i = 0; i < strlen(check_button); i++) {
    if (check_button[i] == '1') {
      a->button1 = true;
    }

    if (check_button[i] == '2') {
      a->button2 = true;
    }

    if (check_button[i] == '3') {
      a->button3 = true;
    }
  }

  a->modmask = modkey;

  a->cmd = cmd;
  a->arg = arg;

  if (pointer->action_binds == NULL) {
    pointer->action_binds = a;
  } else {
    PointerUpdateBind *p = pointer->action_binds;

    while (p) {
      if (p->next == NULL) {
        p->next = a;

        break;
      }

      p = p->next;
    }
  }
}
