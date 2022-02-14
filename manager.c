#include "debug.h"
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include "manager.h"

static unsigned int numlockmask = 0;

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define CLEANMASK(mask)                                                        \
  (mask & ~(numlockmask | LockMask) &                                          \
   (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask |      \
    Mod5Mask))
#define MAX_WINDOW = 100

#include "session.h"

#include "layout.h"

#include "config.h"

Manager_setting *manager_setting_create() {
  Manager_setting *setting = malloc(sizeof(Manager_setting));
  setting->stack_width_percent = STACK_WIDTH_PERCENT;
  setting->gap_width = GAP_WIDTH;
  setting->border_width = BORDER_WIDTH;
  setting->border_color_active = BORDER_COLOR_ACTIVE;
  setting->border_color_inactive = BORDER_COLOR_INACTIVE;

  int i;

  for (i = 0; i < LENGHT(KEYS); i++) {
    setting->keys[i] = KEYS[i];
  };
  memcpy(setting->keys, KEYS, LENGTH(KEYS));

  return setting;
}

Manager_session *manager_start_session(xcb_connection_t *conn,
                                       xcb_screen_t *screen,
                                       Manager_setting *setting) {
  Manager_session *ms = malloc(sizeof(Manager_session));

  ms->conn = conn;
  ms->screen = screen;

  ms->window_count = 0;
  ms->active_window = 0;
  ms->setting = manager_setting_create();
  return ms;
}

Manager_window *manager_create_window(Manager_session *ms,
                                      xcb_window_t window) {
  Manager_window *mw = malloc(sizeof(Manager_window));
  mw->id = mw->id++;
  mw->window = window;
  mw->width = 500;
  mw->height = 300;
  mw->x = 30;
  mw->y = 20;

  ms->window[ms->window_count++] = mw;

  ms->active_window = mw->id;

  manager_window_update(ms, mw);

  manager_layout_update(manager_session);

  return mw;
}

void manager_move_window(Manager_session *manager_session,
                         Manager_window *window, int x, int y) {
  window->x = x;
  window->y = y;
  manager_window_update(manager_session, window);
}

void manager_window_update(Manager_session *manager_session,
                           Manager_window *window) {
  int vals[5];

  vals[0] = window->x + manager_session->setting->gap_width;
  vals[1] = window->y + manager_session->setting->gap_width;
  vals[2] = window->width - manager_session->setting->gap_width * 2;
  vals[3] = window->height - manager_session->setting->gap_width * 2;
  vals[4] = 2;
  xcb_configure_window(manager_session->conn, window->window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
                           XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       vals);

  if (window->id == manager_session->active_window) {
    vals[0] = manager_session->setting->border_color_active;
  } else {
    vals[0] = manager_session->setting->border_color_inactive;
  }

  xcb_change_window_attributes(manager_session->conn, window->window,
                               XCB_CW_BORDER_PIXEL, vals);

  xcb_flush(manager_session->conn);
}

void manager_trigger_key(Manager_session *manager_session, xcb_keysym_t keysym,
                         uint16_t state) {
  int i;
  i = 0;
  for (i = 0; i < LENGTH(KEYS); i++) {
    if (keysym == manager_session->setting->keys[i].keysym &&
        CLEANMASK(state) ==
            CLEANMASK(manager_session->setting->keys[i].modifiers)) {
      manager_session->setting->keys[i].func(
          &(manager_session->setting->keys[i].arg));
      break;
    }
  }
}

void manager_layout_update(Manager_session *manager_session) {
  layout_stack(manager_session);
}

void manager_run(const Manager_arg *arg) {
  int status;

  if (fork() == 0) {
    status = system(arg->v);
    exit(0);
  }
}
