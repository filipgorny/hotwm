#include "layout.h"
#include "client.h"
#include "session.h"
#include "style.h"

void layout_mono_apply(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client, Style *style) {
    Client *c = clients;

    while (c) {
        c->window->hidden = true;
        window_update(c->window);

        c = c->next;
    }

    if (current_client == NULL) {
        return;
    }

    current_client->window->hidden = false;

    current_client->window->x = style_get(style, "margin");
    current_client->window->y  = style_get(style, "margin");
    current_client->window->width = screen->width_in_pixels - style_get(style, "margin") * 2;
    current_client->window->height = screen->height_in_pixels - style_get(style, "margin") * 2;

    window_update(current_client->window);
}

