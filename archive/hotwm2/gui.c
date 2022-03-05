#include <stdlib.h>
#include <sys/types.h>
#include <xcb/xcb.h>

#include "client.h"

#include "config.h"
#include "gui.h"

void gui_decorate_client(Client *client) {
  uint32_t color, titlebar, titletext = 0;

  if (client->is_selected) {
    color = COLOR_BORDER_ACTIVE;
    titlebar = COLOR_TITLE_ACTIVE;
    titletext = COLOR_TITLE_TEXT_ACTIVE;

  } else {
    color = COLOR_BORDER_INACTIVE;
    titlebar = COLOR_TITLE_INACTIVE;
    titletext = COLOR_TITLE_TEXT_INACTIVE;
  }

  draw_rect(client->parent, 0, 0, client->width, client->height, color);
  draw_rect(client->parent, 1, 1, client->width - 2, TITLE_BAR_HEIGHT - 2,
            titlebar);

  char *title = client_get_name(client);

  draw_text(client->parent, 8, 18, title, titletext, titlebar,
            "Roboto-Regular");
}
