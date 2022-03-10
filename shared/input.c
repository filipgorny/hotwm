#include "input.h"
#include "action.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

static unsigned int numlockmask = 0;

#define CLEANMASK(mask)                                                        \
  (mask & ~(numlockmask | LockMask) &                                          \
   (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask |      \
    Mod5Mask))

InputConfig *input_create_config(xcb_connection_t *conn) {
  InputConfig *config = malloc(sizeof(InputConfig));
  config->key_index = 0;
  config->conn = conn;

  return config;
}

void input_define_key(InputConfig *config, xcb_keysym_t keysym,
                      unsigned int modifiers, void (*func)(Arg *arg),
                      Arg *arg) {
  Key *key = malloc(sizeof(Key));
  key->keysym = keysym;
  key->modifiers = modifiers;
  key->func = func;
  key->action_name = NULL;

  key->arg = malloc(sizeof(Arg));
  key->arg->v = arg->v;
  key->arg->i = arg->i;

  config->keys[config->key_index++] = key;
}

void input_handle_key_event(InputConfig *config,
                            ActionsRegistry *actions_registry,
                            xcb_key_press_event_t *event) {
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(config->conn);
  xcb_keysym_t keysym;
  keysym =
      (!(keysyms) ? 0 : xcb_key_symbols_get_keysym(keysyms, event->detail, 0));

  if (keysym == NULL) {
    printf("[KEYBOARD] Unknown keysym\n");

    return;
  }

  printf("[KEYBOARD] key: %d, modifier: %d\n", keysym);

  int i;

  for (i = 0; i < config->key_index; i++) {
    if (keysym == config->keys[i]->keysym &&
        CLEANMASK(event->state) == CLEANMASK(config->keys[i]->modifiers)) {

      printf("[KEYBOARD] Found key, %d\n", config->keys[i]->keysym);

      printf("V: %s\n", config->keys[i]->arg->v);

      if (config->keys[i]->func) {
        config->keys[i]->func(config->keys[i]->arg);
      } else if (config->keys[i]->action_name) {
        action_execute(actions_registry, config->keys[i]->action_name);
      }

      return;
    }
  }
}

void input_config_free(InputConfig *config) {}

void input_define_key_action(InputConfig *config, xcb_keysym_t keysym,
                             unsigned int modifiers, char *action_name) {

  Key *key = malloc(sizeof(Key));
  key->keysym = keysym;
  key->modifiers = modifiers;
  key->func = NULL;
  key->action_name = action_name;

  key->arg = malloc(sizeof(Arg));

  config->keys[config->key_index++] = key;
}
