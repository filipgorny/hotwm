#pragma once

#include "client.h"

#include <stdbool.h>

#define layout_callback void (*apply)(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client)

typedef struct Layout Layout;
struct Layout {
    char *name;
    layout_callback;
};


Layout *layout_create(char *name, layout_callback);
void layout_define_method(Layout *layout, layout_callback);

