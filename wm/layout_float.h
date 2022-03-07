#include "layout.h"

void layout_stack_float(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client) {
   Client *c = clients;

   while (c) {
       c = c->next;
   }
}
