#include "layout.h"
#include "window.h"

void layout_stack_apply(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client, Style *style) {
  Client *c = clients;
  int total_clients = 0;

    while (c) {
        total_clients++;

        c = c->next;
    }

    int stack_window_height = (screen->height_in_pixels - style->margin * 2) / total_clients - style->gap;
    int stack_window_width = 1.618 * (screen->width_in_pixels - 2 * style->margin);

    int stack_index = 0;

    int current_window_height = 200; //screen->height_in_pixels - style->margin * 2 - 400;
    int current_window_width = 200; //screen->width_in_pixels - stack_window_width - 2 * style->margin - style->gap - 400;

    Client *client = clients;

    while (client) {
        client->window->hidden = false;
        client->window->width = stack_window_width;
        client->window->height = stack_window_height;
        client->window->x = style->margin;
        client->window->y = style->margin + stack_index * (stack_window_height + style->gap);

        window_update(client->window);

        client = client->next;
    }

    if (current_client == NULL) {
        return;
    }

    current_client->window->hidden = false;

    current_client->window->x = stack_window_width + style->gap;
    current_client->window->y  = style->margin;
    current_client->window->width = current_window_width;
    current_client->window->height = current_window_height;

    window_update(current_client->window);
}
