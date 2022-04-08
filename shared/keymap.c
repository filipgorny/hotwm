#include "keymap.h"
#include "log.h"

#include <X11/keysym.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

#define KEYSYMBOLS_LENGTH 90

KeySymbol keysymbols[KEYSYMBOLS_LENGTH] = {{"a", XK_a},
                                           {"b", XK_b},
                                           {"c", XK_c},
                                           {"d", XK_d},
                                           {"e", XK_e},
                                           {"f", XK_f},
                                           {"g", XK_g},
                                           {"h", XK_h},
                                           {"i", XK_i},
                                           {"j", XK_j},
                                           {"k", XK_k},
                                           {"l", XK_l},
                                           {"m", XK_m},
                                           {"n", XK_n},
                                           {"o", XK_o},
                                           {"p", XK_p},
                                           {"q", XK_q},
                                           {"r", XK_r},
                                           {"s", XK_s},
                                           {"t", XK_t},
                                           {"u", XK_u},
                                           {"v", XK_v},
                                           {"w", XK_w},
                                           {"x", XK_x},
                                           {"y", XK_y},
                                           {"z", XK_z},
                                           {"1", XK_1},
                                           {"2", XK_2},
                                           {"3", XK_3},
                                           {"4", XK_4},
                                           {"5", XK_5},
                                           {"6", XK_6},
                                           {"7", XK_7},
                                           {"8", XK_8},
                                           {"9", XK_9},
                                           {"0", XK_0},
                                           {"F1", XK_F1},
                                           {"F2", XK_F2},
                                           {"F3", XK_F3},
                                           {"F4", XK_F4},
                                           {"F5", XK_F5},
                                           {"F6", XK_F6},
                                           {"F7", XK_F7},
                                           {"F8", XK_F8},
                                           {"F9", XK_F9},
                                           {"F10", XK_F10},
                                           {"F11", XK_F11},
                                           {"F12", XK_F12},
                                           {"R", XK_Return},
                                           {"E", XK_Escape},
                                           {"T", XK_Tab},
                                           {"`", XK_grave},
                                           {"-", XK_minus},
                                           {"=", XK_equal},
                                           {"[", XK_bracketleft},
                                           {"]", XK_bracketright},
                                           {"\\", XK_backslash},
                                           {";", XK_semicolon},
                                           {"'", XK_apostrophe},
                                           {",", XK_comma},
                                           {".", XK_period},
                                           {"/", XK_slash},
                                           {"space", XK_space},
                                           {"up", XK_Up},
                                           {"down", XK_Down},
                                           {"left", XK_Left},
                                           {"right", XK_Right},
                                           {"home", XK_Home},
                                           {"end", XK_End},
                                           {"pageup", XK_Page_Up},
                                           {"pagedown", XK_Page_Down},
                                           {"insert", XK_Insert},
                                           {"delete", XK_Delete},
                                           {"backspace", XK_BackSpace},
                                           {"enter", XK_Return},
                                           {"tab", XK_Tab},
                                           {"capslock", XK_Caps_Lock},
                                           {"numlock", XK_Num_Lock},
                                           {"scrolllock", XK_Scroll_Lock},
                                           {"pause", XK_Pause},
                                           {"printscreen", XK_Print}};

Keymap *keymap_create() {
  Keymap *keymap = (Keymap *)malloc(sizeof(Keymap));
  keymap->keybinds = NULL;

  return keymap;
}

void keymap_bind(Keymap *keymap, char *key, char *cmd, Arg arg) {
  xcb_mod_mask_t modkey;
  char *key_char;
  int i, ii;
  bool enable_character_lookup = false;
  bool key_char_defined = false;

  for (i = 0; i < strlen(key); i++) {
    if (enable_character_lookup) {
      key_char = (char *)malloc(strlen(key) - i + 1);

      for (ii = i; ii < strlen(key); ii++) {
        key_char[ii - i] = key[ii];
      }

      key_char[ii - i] = '\0';

      key_char_defined = true;
    }

    if (key[i] == 'S') {
      modkey = XCB_MOD_MASK_1;
    }

    if (key[i] == '-') {
      enable_character_lookup = true;
    }
  }

  if (enable_character_lookup) {
    for (i = 0; i < KEYSYMBOLS_LENGTH; i++) {
      if (strcmp(keysymbols[i].symbol, key_char) == 0) {
        Keybind *keybind = (Keybind *)malloc(sizeof(Keybind));
        keybind->key = keysymbols[i];
        keybind->cmd = cmd;
        keybind->arg = arg;
        keybind->modkey = modkey;
        keybind->next = NULL;

        if (keymap->keybinds == NULL) {
          keymap->keybinds = keybind;
        } else {
          Keybind *keybind_tmp = keymap->keybinds;
          while (keybind_tmp) {
            if (keybind_tmp->next == NULL) {
              keybind_tmp->next = keybind;
              break;
            }

            keybind_tmp = keybind_tmp->next;
          }
        }

        break;
      }
    }
  }
}

Keybind *keymap_get_keybind(Keymap *keymap, xcb_keysym_t keysym,
                            xcb_mod_mask_t modkey) {
  Keybind *keybind = keymap->keybinds;

  while (keybind != NULL) {
    if (keybind->key.keysym == keysym && keybind->modkey == modkey) {
      return keybind;
    }

    keybind = keybind->next;
  }

  return NULL;
}
