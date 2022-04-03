#include "gui.h"
#include "draw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

Gui *gui_initialize(Draw *draw) {
  Gui *gui = malloc(sizeof(Gui));
  gui->draw = draw;
  gui->font = "Monospace 16";

  return gui;
}

void gui_draw_element(Gui *gui, Element *element) {
  element->draw_func(gui, element, 0, 0);
}

static Property *_get_prop(Element *element, char *name) {
  printf("-* ELEMENT num prop %d\n", element->num_properties);

  for (int i = 0; i < element->num_properties; i++) {
    printf(" -- prop %s - %s\n", element->properties[i].name, name);
    if (strcmp(element->properties[i].name, name) == 0) {
      return &element->properties[i];
    }
  }

  return NULL;
}

static void _add_prop(Element *element, char *name, Arg arg) {
  Property *p = malloc(sizeof(Property));
  p->name = name;
  p->value = &arg;

  element->properties[element->num_properties] = *p;
  element->num_properties++;
}

void gui_add_child(Element *parent, Element *child) {
  if (parent->children == NULL) {
    printf("new children\n");
    parent->children = child;

    return;
  }
  printf("adding children\n");
  Element *current = parent->children;

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

void gui_remove_child(Element *parent, Element *child) {
  if (parent->children == child) {
    parent->children = child->next;
  } else {
    Element *current = parent->children;

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

void gui_remove_child_by_meta_id(Element *parent, int meta_id) {
  Element *current = parent->children;

  while (current != NULL) {
    if (current->meta_id == meta_id) {
      gui_remove_child(parent, current);

      break;
    }

    current = current->next;
  }
}
static void _draw_container(Gui *gui, Element *element, int offset_x,
                            int offset_y) {
  draw_rect(gui->draw, element->window, element->x + offset_x,
            element->y + offset_y, element->width, element->height, 0x00ff0000);

  Element *child = element->children;
  printf("drawing\n");
  while (child != NULL) {
    printf("drawing child\n");
    if (child->draw_func != NULL) {
      printf("child has draw func\n");
      child->draw_func(gui, child, element->x, element->y);
    }

    child = child->next;
  }
}

Element *gui_container(Gui *gui, xcb_window_t window, int x, int y, int width,
                       int height) {
  Element *element = malloc(sizeof(Element));
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
  element->properties = malloc(sizeof(Property) * 1);

  return element;
}

static void _draw_button(Gui *gui, Element *element, int offset_x,
                         int offset_y) {
  draw_rect(gui->draw, element->window, element->x + offset_x,
            element->y + offset_y, element->width, element->height, 0x00111111);

  draw_rect(gui->draw, element->window, element->x + offset_x + 1,
            element->y + offset_y + 1, element->width - 2, element->height - 2,
            0x00333333);

  printf("DRAWING BUTTON\n");

  Property *prop = _get_prop(element, "label");

  if (prop != NULL) {
    printf("Drawing text\n");
    draw_text(gui->draw, element->window, element->x + offset_x,
              element->y + offset_y, prop->value->v, 0x00ffffff, 0x00333333,
              gui->font);
  }
}

Element *gui_button(Gui *gui, xcb_window_t window, int x, int y, int w, int h,
                    char *label) {
  Element *element = malloc(sizeof(Element));
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
  element->properties = malloc(sizeof(Property) * 1);

  _add_prop(element, "label", (Arg){.v = label});

  return element;
}
