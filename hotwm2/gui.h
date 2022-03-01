#include "draw.h"
#include "client.h"

#ifndef GUI_H
#define GUI_H

/*
typedef struct {
    int x, y, w, h;
} WidgetProperties;

typedef void (*draw)(const WidgetProperties properties);

typedef struct WidgetDefinition WidgetDefinition;

typedef struct {
    char* name;
    draw* draw;
    WidgetDefinition* next;
} WidetDefinition;

void gui_define_widget(const char* name, draw draw_func);
*/

void gui_decorate_client(Client *client);

#endif
