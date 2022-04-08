#include "panel.h"
#include "draw.h"
#include "gui.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

#include "toolkit.hpp"

static int current_panel_id = 1;
static Draw *draw;
static Gui *gui;

static void _add_task(Panel *panel, Task *task) {
  printf("adding task - %s\n", task->name);

  if (panel->tasks == NULL) {
    panel->tasks = task;
  } else {
    Task *current = panel->tasks;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = task;
  }

  panel->tasks_count++;
  printf("count = %d\n", panel->tasks_count);

  int base_width;

  if (panel->tasks_count > 4) {
    base_width = panel->gui_container_tasks->width / panel->tasks_count;
  } else {
    base_width = panel->gui_container_tasks->width / 4;
  }

  int button_width = floor(base_width * 0.6);
  int button_left =
      floor(base_width * (panel->tasks_count - 1) + base_width * 0.2);

  printf("container width = %d\n", panel->gui_container_tasks->width);
  printf("base width = %d\n", base_width);
  printf("button_left: %d\n", button_left);
  printf("button_width: %d\n", button_width);

  char *button_text;

  if (task->name == NULL) {
    button_text = "";
  } else {
    button_text = (char *)malloc(sizeof(char) * (strlen(task->name) + 1));
    strcpy(button_text, task->name);
  }

  GuiElement *gui_button_task =
      gui_button(gui, panel->window, button_left, 0, button_width,
                 panel->height - panel->padding * 2, button_text);

  gui_button_task->meta_id = task->window;

  gui_add_child(panel->gui_container_tasks, gui_button_task);

  View view = toolkit_generate(*panel->gui_container_tasks);
}

static void _remove_task(Panel *panel, Task *task) {
  bool found = false;

  if (panel->tasks == task) {
    panel->tasks = task->next;
    found = true;
  } else {
    Task *current = panel->tasks;

    if (current == NULL) {
      return;
    }

    while (current->next != task) {
      current = current->next;
    }

    if (current->next == task) {
      current->next = task->next;
      found = true;
    }
  }

  if (!found) {
    return;
  }

  gui_remove_child_by_meta_id(panel->gui_container_tasks, task->window);

  panel->tasks_count--;
}

Panel *panel_create(xcb_connection_t *conn, xcb_screen_t *screen,
                    xcb_window_t root, int x, int y, int w, int h, Draw *draw) {
  Panel *p = (Panel *)malloc(sizeof(Panel));
  p->id = current_panel_id++;
  p->tasks_count = 0;

  xcb_window_t window = xcb_generate_id(conn);

  xcb_create_window(conn, screen->root_depth, window, root, x, y, w, h, 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0,
                    NULL);

  xcb_map_window(conn, window);

  xcb_flush(conn);

  draw_rect(draw, window, 0, 0, w, h, 0x444444);

  p->window = window;

  p->height = h;
  p->padding = 0;
  p->tasks = NULL;

  if (!gui) {
    draw = draw_init(conn, screen, &screen->root);
    gui = gui_initialize(draw);
  }

  p->gui_container_tasks = gui_container(
      gui, p->window, p->padding, p->padding,
      screen->width_in_pixels - p->padding * 2, p->height - p->padding * 2);

  return p;
}

void panel_update_tasks(Panel *panel, Task *tasks) {
  Task *task = tasks;
  Task *panel_task = panel->tasks;

  bool found = false;

  // check if not present
  if (panel->tasks == NULL) {
    _add_task(panel, task);
  } else {
    while (task != NULL) {
      found = false;

      while (panel_task != NULL) {
        if (task->window == panel_task->window) {
          found = true;
        }

        panel_task = panel_task->next;
      }

      if (!found) {
        _add_task(panel, task);
      }

      task = task->next;
    }
  }

  // check if removed
  while (panel_task != NULL) {
    found = false;

    while (task != NULL) {
      if (task->window == panel_task->window) {
        found = true;
        break;
      }

      task = task->next;
    }

    if (!found) {
      _remove_task(panel, panel_task);
    }

    panel_task = panel_task->next;
  }
}

void panel_draw_gui(Panel *panel) {
  gui_draw_element(gui, panel->gui_container_tasks, 0, 0);
}
