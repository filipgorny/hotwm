#include "client.h"
#include "input.h"
#include "layout.h"
#include "mouse.h"
#include <xcb/xproto.h>

#ifndef MAIN_H
#define MAIN_H

static unsigned int numlockmask = 0;

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define CLEANMASK(mask)                                                        \
  (mask & ~(numlockmask | LockMask) &                                          \
   (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask |      \
    Mod5Mask))
typedef struct {
  Client *clients;
} Desktop;
typedef struct Monitor Monitor;
struct Monitor {
  int num;
  int mx, my, mw, mh; // screen size
  Monitor *next;
  Desktop *desktops;
  Desktop *current_desktop;
  enum layout layout
};

typedef struct {
  Monitor *monitors;
  Monitor *current_monitor;
  Client *main_client;
  Client *selected_client;
  Mouse *mouse;
} Session;

static void spawn(const Arg *arg);
static Session *init_session();
static Monitor *create_monitor(int num);
static void trigger_key_bind(xcb_keysym_t keysym, uint16_t state);
static void handle_map_request(xcb_window_t window);
static void handle_key_press(xcb_key_press_event_t *event);
static void handle_button_press(xcb_button_press_event_t *ev);
static void handle_button_release(xcb_button_release_event_t *ev);
static void handle_motion_notify(xcb_motion_notify_event_t *ev);
static void update_client_geometry(Client *c);
static void layout_stacked();
static void active_up();
static void active_down();
static void redraw();
static void toggle();
static void layout_mono();
static void set_layout(const Arg *arg);
static void toggle_floating(const Arg *arg);
static void handle_mouse_motion(xcb_motion_notify_event_t *event);
static xcb_window_t create_parent(Client *client);
static Client *session_get_client_by_window(xcb_window_t window);
static Client *session_get_client_by_cords(int x, int y);
static void session_select_client(Client *client);

#endif
