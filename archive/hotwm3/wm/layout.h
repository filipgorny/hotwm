#pragma once

#include "client.h"
#include "style.h"

#include <stdbool.h>

#define layout_callback void (*apply)(Layout *layout, xcb_screen_t *screen, Client *clients, Client *current_client, Style *style)

typedef struct Layout Layout;
struct Layout {
    char *name;
    layout_callback;
};


Layout *layout_create(char *name, layout_callback);
void layout_define_method(Layout *layout, layout_callback);

