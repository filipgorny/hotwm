#include "gui.h"
#include "draw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

static Property *_get_prop(GuiElement *element, char *name) {

  for (int i = 0; i < element->num_properties; i++) {
    if (strcmp(element->properties[i].name, name) == 0) {
      return &element->properties[i];
    }
  }

  return NULL;
}

static void _add_prop(GuiElement *element, char *name, Arg arg) {
  Property *p = (Property *)malloc(sizeof(Property));
  p->name = name;
  p->value = arg;

  element->properties[element->num_properties] = *p;
  element->num_properties++;
}

Gui *gui_initialize(Draw *draw) {
  Gui *gui = (Gui *)malloc(sizeof(Gui));
  gui->draw = draw;
  gui->font = "Monospace 16";

  return gui;
}

void gui_draw_element(Gui *gui, GuiElement *element, int offset_x,
                      int offset_y) {
  element->draw_func(gui, element, offset_x, offset_y);
}

void gui_add_child(GuiElement *parent, GuiElement *child) {
  if (parent->children == NULL) {
    parent->children = child;

    return;
  }
  GuiElement *current = parent->children;

  while (current->next != NULL) {
    if (current->next == child) {
      return;
    }

    if (current->next == NULL) {
      current->next = child;
      return;
    }

    current = current->next;
  }
}

void gui_remove_child(GuiElement *parent, GuiElement *child) {
  if (parent->children == child) {
    parent->children = child->next;
  } else {
    GuiElement *current = parent->children;

    while (current->next != NULL) {
      if (current->next == child) {
        current->next = child->next;

        break;
      }

      current = current->next;
    }
  }

  free(child);
}

void gui_remove_child_by_meta_id(GuiElement *parent, int meta_id) {
  GuiElement *current = parent->children;

  while (current != NULL) {
    if (current->meta_id == meta_id) {
      gui_remove_child(parent, current);

      break;
    }

    current = current->next;
  }
}
static void _draw_container(Gui *gui, GuiElement *element, int offset_x,
                            int offset_y) {
  draw_rect(gui->draw, element->window, element->x + offset_x,
            element->y + offset_y, element->width, element->height, 0x00000000);

  GuiElement *child = element->children;

  while (child != NULL) {

    gui_draw_element(gui, child, element->x, element->y);

    child = child->next;
  }
}

GuiElement *gui_container(Gui *gui, xcb_window_t window, int x, int y,
                          int width, int height) {
  GuiElement *element = (GuiElement *)malloc(sizeof(GuiElement));
  element->type_name = "container";
  element->next = NULL;
  element->x = x;
  element->y = y;
  element->width = width;
  element->height = height;
  element->draw_func = _draw_container;
  element->window = window;
  element->children = NULL;
  element->num_properties = 0;
  element->properties = (Property *)malloc(sizeof(Property) * 1);

  return element;
}

static void _draw_button(Gui *gui, GuiElement *element, int offset_x,
                         int offset_y) {
  draw_rect(gui->draw, element->window, element->x + offset_x,
            element->y + offset_y, element->width, element->height, 0x00111111);

  draw_rect(gui->draw, element->window, element->x + offset_x + 1,
            element->y + offset_y + 1, element->width - 2, element->height - 2,
            0x00333333);

  Property *prop = _get_prop(element, "label");

  if (prop != NULL) {
    draw_text(gui->draw, element->window, element->x + offset_x,
              element->y + offset_y, prop->value.v, 0x00ffffff, 0x00333333,
              gui->font);
  }
}

GuiElement *gui_button(Gui *gui, xcb_window_t window, int x, int y, int w,
                       int h, char *label) {
  GuiElement *element = (GuiElement *)malloc(sizeof(GuiElement));
  element->type_name = "button";
  element->next = NULL;
  element->x = x;
  element->y = y;
  element->width = w;
  element->height = h;
  element->draw_func = _draw_button;
  element->window = window;
  element->children = NULL;
  element->num_properties = 0;
  element->properties = (Property *)malloc(sizeof(Property) * 1);

  char *label_copy = (char *)malloc(sizeof(char) * strlen(label) + 1);
  strcpy(label_copy, label);

  _add_prop(element, "label", (Arg){.v = label_copy});

  return element;
}
