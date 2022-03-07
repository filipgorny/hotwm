#include "layout.h"
#include "client.h"
#include "session.h"

void layout_mono_apply(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client) {
    if (current_client == NULL) {
        return;
    }

    window_resize(current_client->window, screen->width_in_pixels, screen->height_in_pixels);
}

