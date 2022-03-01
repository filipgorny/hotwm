#include <sys/types.h>
#include <xcb/xcb.h>


#ifndef CLIENT_H
#define CLIENT_H
typedef struct Client Client;
struct Client {
  xcb_window_t window;
  xcb_window_t parent;
  char *name;
  Client *next;
  int is_floating, is_maximized, is_open;
  int x, y, width, height;
  int floating_x, floating_y, floating_width, floating_height, floating_initialized;
  int is_selected;
  xcb_connection_t *conn;
  int display_offset_x, display_offset_y;
  u_int64_t index;
};

char *client_get_name(Client *client);
Client *client_put_on_top(Client *client, Client *target);
int client_count(Client *client);
Client *client_find_parent(Client *client, Client *child);

#endif
