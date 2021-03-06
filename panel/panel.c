#include "panel.h"
#include "draw.h"
#include "gui.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

#include "toolkit.h"

static int current_panel_id = 1;
static Draw *draw;
static Gui *gui;

static ViewDef *_create_view_def() {
  ViewDef *def = (ViewDef *)malloc(sizeof(ViewDef));
  def->root = toolkit_generate_component_def(COMPONENT_CONTAINER);

  ComponentDef *def_tasks_container =
      toolkit_generate_component_def(COMPONENT_CONTAINER);

  ComponentDef *button_start = toolkit_generate_component_def(COMPONENT_BUTTON);
  ComponentDef *button_menu = toolkit_generate_component_def(COMPONENT_BUTTON);

  return def;
}

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

  View *view = toolkit_generate_view(_create_view_def());

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

void panel_draw_gui(Panel *panel) {}
