#include "layout.h"
#include "client.h"
#include "session.h"

void layout_mono_apply(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client) {
    if (current_client == NULL) {
        return;
    }

    current_client->window->width = screen->width_in_pixels;
    current_client->window->height = screen->height_in_pixels;

    window_update(current_client->window);
}

