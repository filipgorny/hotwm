#include "layout.h"
#include "client.h"
#include "session.h"
#include "style.h"

void layout_mono_apply(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client, Style *style) {
    if (current_client == NULL) {
        return;
    }

    current_client->window->x = style->margin;
    current_client->window->y  = style->margin;
    current_client->window->width = screen->width_in_pixels - style->margin * 2;
    current_client->window->height = screen->height_in_pixels - style->margin * 2;

    window_update(current_client->window);
}

