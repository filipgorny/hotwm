#include "layout.h"
#include "window.h"

void layout_stack_float(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client) {
    Client *c = clients;

    while (c) {
        if (c->has_float_cords) {
            c->window->x = c->float_x;
            c->window->y = c->float_y;
            c->window->width = c->float_width;
            c->window->height = c->float_height;

            window_update(c->window);
        } else {
            c->window->width = c->float_width = screen->width_in_pixels / 2;
            c->window->height = c->float_height = screen->height_in_pixels / 2;
            c->window->x = c->float_x = screen->width_in_pixels / 4;
            c->window->y = c->float_y = screen->height_in_pixels / 4;
            c->has_float_cords = true;
        }

        c = c->next;
    }
}
