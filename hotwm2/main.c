#include <X11/Xutil.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/randr.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

static unsigned int numlockmask = 0;

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define CLEANMASK(mask)                                                        \
  (mask & ~(numlockmask | LockMask) &                                          \
   (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask |      \
    Mod5Mask))

const xcb_setup_t *setup;
xcb_generic_event_t *ev;

xcb_drawable_t root;

static xcb_connection_t *conn;
static xcb_screen_t *screen;

typedef struct Client Client;
struct Client {
  xcb_window_t window;
  char name[255];
  Client *next;
  int is_floating, is_maximized, is_open;
};

typedef struct {
  char name[255];
  Client *clients;
} Desktop;

typedef struct Monitor Monitor;
struct Monitor {
  int num;
  int mx, my, mw, mh; // screen size
  int gapx;
  Monitor *next;
  Desktop *desktops;
  Desktop *current_desktop;
};

typedef struct {
  Monitor *monitors;
  Client *clients;
  Monitor *current;
} Session;

typedef union {
  int i;
  unsigned int ui;
  float f;
  char *v;
} Arg;

typedef struct {
  unsigned int modifiers;
  xcb_keysym_t keysym;
  void (*func)(const Arg *);
  const Arg arg;
} Key;

static void spawn(const Arg *arg);
static Session *init_session();
static Monitor *create_monitor(int num);
static void trigger_key_bind(xcb_keysym_t keysym, uint16_t state);
static void handle_map_request(xcb_window_t window);
static void handle_key_press(xcb_key_press_event_t *event);
static void handle_button_release(xcb_generic_event_t *ev);

#include "config.h"

Session *session;

Session *init_session() {
  session = malloc(sizeof(Session));
  session->monitors = malloc(sizeof(Monitor));
  session->clients = NULL;
  session->current = session->monitors;

  return session;
}

Monitor *create_monitor(int num) {
  Monitor *m = malloc(sizeof(Monitor));
  m->num = num;
  m->mx = 0;
  m->my = 0;
  m->mw = 0;
  m->mh = 0;
  m->gapx = 0;
  m->next = NULL;
  m->desktops = malloc(sizeof(Desktop));
  m->current_desktop = m->desktops;

  return m;
}

void spawn(const Arg *arg) {
  int status;

  if (fork() == 0) {
    status = system(arg->v);
    exit(0);
  }
}

Client *create_client(xcb_window_t window) {
  Client *c = malloc(sizeof(Client));
  c->window = window;
  c->next = NULL;
  c->is_floating = 0;
  c->is_maximized = 0;
  c->is_open = 1;

  return c;
}

void handle_map_request(xcb_window_t window) {
  xcb_map_window(conn, window);

  if (session->clients) {
    Client *c = session->clients;
    while (c->next) {
      c = c->next;
    }
    c->next = create_client(window);
  } else {
    session->clients = create_client(window);
  }
  session->clients = create_client(window);

  xcb_flush(conn);
}

void handle_key_press(xcb_key_press_event_t *event) {
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
  xcb_keysym_t keysym;
  keysym =
      (!(keysyms) ? 0 : xcb_key_symbols_get_keysym(keysyms, event->detail, 0));
  xcb_key_symbols_free(keysyms);

  int i;

  for (i = 0; i < LENGTH(KEYS); i++) {
    if (keysym == KEYS[i].keysym &&
        CLEANMASK(event->state) == CLEANMASK(KEYS[i].modifiers)) {
      KEYS[i].func(&(KEYS[i].arg));
      break;
    }
  }

  xcb_flush(conn);
}

void handle_button_release(xcb_generic_event_t *ev) { xcb_flush(conn); }

int main() {
  conn = xcb_connect(NULL, NULL);

  int ret = xcb_connection_has_error(conn);

  if (ret > 0) {
    exit(ret);
  }

  setup = xcb_get_setup(conn);
  screen = xcb_setup_roots_iterator(setup).data;
  xcb_flush(conn);

  root = screen->root;

  session = init_session();

  int values[3];

  values[0] =
      XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
      XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE;
  xcb_change_window_attributes_checked(conn, root, XCB_CW_EVENT_MASK, values);

  xcb_grab_key(conn, 1, root, XCB_MOD_MASK_ANY, XCB_GRAB_ANY,
               XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
  xcb_grab_button(conn, 1, root,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, root, XCB_NONE,
                  XCB_BUTTON_INDEX_1, XCB_MOD_MASK_ANY);

  while (1) {
    xcb_flush(conn);
    ev = xcb_wait_for_event(conn);

    switch (ev->response_type) {
    case 5: // Button release event
      handle_button_release(ev);
      break;
    case XCB_KEY_PRESS:
      printf("Keyboard Pressed\n");
      xcb_key_press_event_t *key_event = (xcb_key_press_event_t *)ev;
      handle_key_press(key_event);
      break;
    case XCB_MAP_REQUEST:
      printf("Map request\n");
      xcb_map_request_event_t *map_event = (xcb_map_request_event_t *)ev;
      handle_map_request(map_event->window);
      break;
    default:
      printf("Unknown event: %d\n", ev->response_type);
      xcb_flush(conn);
      break;
    }
  }

  return 0;
}
