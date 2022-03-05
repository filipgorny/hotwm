#include "input.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <locale.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include <X11/keysym.h>
#include <xcb/xproto.h>

static unsigned int numlockmask = 0;

#define CLEANMASK(mask)                                                        \
  (mask & ~(numlockmask | LockMask) &                                          \
   (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask |      \
    Mod5Mask))

InputConfig *input_create_config(xcb_connection_t *conn) {
  InputConfig *config = malloc(sizeof(InputConfig));
  config->key_index = 0;
  config->keysyms = xcb_key_symbols_alloc(conn);

  return config;
}

void input_define_key(InputConfig *config, xcb_keysym_t kesym,
                      unsigned int modifiers, void (*func)(const Arg *),
                      Arg arg) {
  Key *key = malloc(sizeof(Key));
  key->keysym = kesym;
  key->modifiers = modifiers;
  key->func = func;
  key->arg = arg;

  config->keys[++config->key_index] = *key;
}

void input_handle_key_event(InputConfig *config, xcb_key_press_event_t *event) {
  xcb_keysym_t keysym;
  keysym = xcb_key_symbols_get_keysym(config->keysyms, event->detail, 0);

  int i;
  int keycode, number;

  for (i = 0; i < config->key_index; i++) {
    if (keysym == config->keys[i].keysym &&
        CLEANMASK(event->state) == CLEANMASK(config->keys[i].modifiers)) {
      config->keys[i].func(&(config->keys[i].arg));

      return;
    }
  }
}

void input_config_free(InputConfig *config) {
  xcb_key_symbols_free(config->keysyms);
}
