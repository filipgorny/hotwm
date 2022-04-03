#pragma once

#include "arg.h"
#include "draw.h"
#include <xcb/xproto.h>

#define color u_int32_t

typedef struct {
    Draw *draw;
    char* font;
} Gui;

typedef struct {
    char* name;
    Arg* value;
} Property;

typedef struct Element Element;
struct Element {
    char* type_name;
    int x, y;
    int width, height;
    int meta_id;
    void (*draw_func)(Gui *gui, Element *element, int offset_x, int offset_y);
    Element* children;
    Element* next;
    Property* properties;
    int num_properties;
    xcb_window_t window;
};

Gui *gui_initialize(Draw *draw);

void gui_draw_element(Gui *gui, Element *element);

void gui_add_child(Element *parent, Element *child);
void gui_remove_child(Element *parent, Element *child);
void gui_remove_child_by_meta_id(Element *parent, int meta_id);

Element *gui_container(Gui *gui, xcb_window_t window, int x, int y, int width, int height);
Element *gui_button(Gui *gui, xcb_window_t window, int x, int y, int w, int h, char *label);
