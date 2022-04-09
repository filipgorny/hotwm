#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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
    Arg value;
} Property;

typedef struct GuiElement GuiElement;
struct GuiElement {
    char* type_name;
    int x, y;
    int width, height;
    int meta_id;
    void (*draw_func)(Gui *gui, GuiElement *element, int offset_x, int offset_y);
    GuiElement* children;
    GuiElement* next;
    Property* properties;
    int num_properties;
    xcb_window_t window;
    void (*on_click)(Gui *gui, GuiElement *element);
};

Gui *gui_initialize(Draw *draw);

void gui_draw_element(Gui *gui, GuiElement *element, int offset_x, int offset_y);

void gui_add_child(GuiElement *parent, GuiElement *child);
void gui_remove_child(GuiElement *parent, GuiElement *child);
void gui_remove_child_by_meta_id(GuiElement *parent, int meta_id);

void gui_fire_click(Gui *gui, int x, int y);

GuiElement *gui_container(Gui *gui, xcb_window_t window, int x, int y, int width, int height);
GuiElement *gui_button(Gui *gui, xcb_window_t window, int x, int y, int w, int h, char *label);

#ifdef __cplusplus
}
#endif


