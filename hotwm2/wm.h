#include "client.h"
#include "input.h"
#include "layout.h"
#include "mouse.h"
#include <sys/types.h>
#include <xcb/xproto.h>

#define false 0
#define true 1

#ifndef MAIN_H
#define MAIN_H

static unsigned int numlockmask = 0;

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define CLEANMASK(mask)                                                        \
  (mask & ~(numlockmask | LockMask) &                                          \
   (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask |      \
    Mod5Mask))

typedef struct Desktop Desktop;
struct Desktop {
  Client *clients;
  xcb_window_t window;
  Desktop *next;
  Client *main_client;
  Client *selected_client;
 
};

typedef struct Monitor Monitor;
struct Monitor {
  int num;
  int mx, my, mw, mh; // screen size
  Monitor *next;
  Desktop *desktops;
  Desktop *current_desktop;
  enum layout layout
};

typedef struct Bookmark Bookmark;
struct Bookmark {
    Client *client;
    Desktop *desktop;
    int number;
    Bookmark *next;
};

typedef struct {
  Monitor *monitors;
  Monitor *current_monitor;
  Mouse *mouse;
  u_int64_t current_index;
  Bookmark* bookmarks;
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
static void session_set_main_client(Client *client);
static void session_raise_client(Client *client);
static void desktop_next();
static void desktop_previous();
static void bookmark_add(int number, Client *client);
static void bookmark_show(int number);

#endif
