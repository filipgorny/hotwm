#include <sys/types.h>
#include <xcb/xcb.h>

#include "client.h"

#include "config.h"
#include "gui.h"

#include "window.h"

void gui_decorate_client(Client *client) {
  uint32_t color = 0;

  if (client->is_selected) {
    color = COLOR_BORDER_ACTIVE;
  } else {
    color = COLOR_BORDER_INACTIVE;
  }

  draw_rect(client->parent, 0, 0, client->width, client->height, color);

  char *title = window_get_name(client);

  printf("titke: %s\n", title);
}
