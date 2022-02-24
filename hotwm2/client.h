#include <xcb/xcb.h>

#ifndef CLIENT_H
#define CLIENT_H
typedef struct Client Client;
struct Client {
  xcb_window_t window;
  xcb_window_t parent;
  char name[255];
  Client *next;
  int is_floating, is_maximized, is_open;
  int x, y, width, height;
  int is_selected;
  xcb_connection_t *conn;
};

#endif
