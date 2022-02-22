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
  int x, y, width, height;
};

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
};

typedef struct {
  Monitor *monitors;
  Monitor *current_monitor;
  Client *current_client;
  Client *selected_client;
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
static void update_client_geometry(Client *c);
static void layout_stacked();
static void active_up();
static void active_down();
static void redraw();
static void toggle();
static void layout_mono();

#include "config.h"

Session *session;

Session *init_session() {
  session = malloc(sizeof(Session));
  session->monitors = create_monitor(0);
  session->current_monitor = session->monitors;
  session->selected_client = NULL;
  session->current_client = NULL;

  return session;
}

Desktop *create_desktop() {
  Desktop *desktop = malloc(sizeof(Desktop));
  desktop->clients = NULL;

  return desktop;
}

Monitor *create_monitor(int num) {
  Monitor *m = malloc(sizeof(Monitor));
  m->num = num;
  m->mx = 0;
  m->my = 0;
  m->mw = screen->width_in_pixels;
  m->mh = screen->height_in_pixels;
  m->next = NULL;
  m->desktops = create_desktop();
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

void active_down() {
  Client *c = session->current_monitor->current_desktop->clients;

  while (c->next) {
    if (c->next == session->selected_client) {
      session->selected_client = c;

      redraw();

      return;
    }

    c = c->next;
  }
}

void active_up() {
  Client *c = session->current_monitor->current_desktop->clients;

  if (!c->next) {
    session->selected_client = c;

    redraw();

    return;
  }

  while (c->next) {
    if (c == session->selected_client) {
      session->selected_client = c->next;

      redraw();

      return;
    }

    c = c->next;
  }
}

void toggle() {
  session->current_client = session->selected_client;
  redraw();

  return;
  Client *c = session->current_monitor->current_desktop->clients;

  while (c->next) {
    if (c == session->selected_client) {
      Client *current = session->current_client;

      session->current_client = c;
      c = current;
      c->next = current->next;

      redraw();

      return;
    }

    c = c->next;
  }
}

void handle_map_request(xcb_window_t window) {
  xcb_map_window(conn, window);

  Client *current_client = create_client(window);

  if (session->current_monitor->current_desktop->clients != NULL) {
    Client *c = session->current_monitor->current_desktop->clients;
    while (c->next) {
      c = c->next;
    }
    c->next = current_client;
  } else {
    session->current_monitor->current_desktop->clients = current_client;
  }

  session->current_client = current_client;
  session->selected_client = current_client;

  redraw();

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

      xcb_flush(conn);

      return;
    }
  }

  if (session->current_client) {
    xcb_send_event(conn, 0, session->selected_client->window,
                   XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *)event);
  }

  xcb_flush(conn);
}

void handle_button_release(xcb_generic_event_t *ev) { xcb_flush(conn); }

void update_client_geometry(Client *c) {
  int vals[5];

  vals[0] = c->x;
  vals[1] = c->y;
  vals[2] = c->width;
  vals[3] = c->height;
  vals[4] = 2;
  xcb_configure_window(conn, c->window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
                           XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       vals);

  if (c == session->selected_client) {
    vals[0] = BORDER_COLOR_ACTIVE;
  } else {
    vals[0] = BORDER_COLOR_INACTIVE;
  }

  xcb_change_window_attributes(conn, c->window, XCB_CW_BORDER_PIXEL, vals);

  xcb_flush(conn);
}

void layout_stacked() {
  Client *c = session->current_monitor->current_desktop->clients;

  int length = 0;

  do {
    if (c->is_open && c != session->current_client) {
      length++;
    }
  } while (c = c->next);

  int window_height = session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  int current_y = BAR_MARGIN + GAP_WIDTH;

  int i;

  c = session->current_monitor->current_desktop->clients;

  while (c) {
    if (c->is_open && c != session->current_client) {
      c->x = GAP_WIDTH;
      c->y = current_y;
      c->width = STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw -
                 GAP_WIDTH;

      c->height = (int)((session->current_monitor->mh - BAR_MARGIN -
                         (length + 1) * GAP_WIDTH) /
                        length);

      update_client_geometry(c);

      current_y += c->height + GAP_WIDTH;
    }

    c = c->next;
  }

  if (length > 0) {
    session->current_client->x =
        STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw + GAP_WIDTH;

    session->current_client->width =
        session->current_monitor->mw - 2 * GAP_WIDTH -
        STACK_WIDTH_PERCENT * 0.01f * session->current_monitor->mw;

  } else {
    session->current_client->x = GAP_WIDTH;

    session->current_client->width =
        session->current_monitor->mw - 2 * GAP_WIDTH;
  }

  session->current_client->y = BAR_MARGIN + GAP_WIDTH;
  session->current_client->height =
      session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  update_client_geometry(session->current_client);
}

void layout_mono() {
  Client *c = session->current_monitor->current_desktop->clients;

  while (c) {
    xcb_unmap_window(conn, c->window);

    c = c->next;
  }

  xcb_map_window(conn, session->selected_client->window);

  session->selected_client->x = GAP_WIDTH;
  session->selected_client->y = BAR_MARGIN + GAP_WIDTH;
  session->selected_client->width =
      session->current_monitor->mw - 2 * GAP_WIDTH;
  session->selected_client->height =
      session->current_monitor->mh - BAR_MARGIN - 2 * GAP_WIDTH;

  update_client_geometry(session->selected_client);
}

void redraw() { layout_mono(); }

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
