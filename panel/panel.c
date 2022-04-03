#include "panel.h"
#include "draw.h"
#include "gui.h"

#include <stdbool.h>
#include <stdlib.h>
#include <xcb/xproto.h>

static int current_panel_id = 1;
static Draw *draw;
static Gui *gui;

static void _add_task(Panel *panel, Task *task) {
  printf("Adding new task\n");
  if (panel->tasks == NULL) {
    panel->tasks = task;
    printf("Added task\n");
    return;
  }

  Task *current = panel->tasks;
  while (current->next != NULL) {
    current = current->next;
  }
  printf("Adding task 2\n");
  current->next = task;

  int button_width = 80;

  Element *gui_button_task =
      gui_button(gui, panel->window, panel->tasks_count * (button_width + 2), 0,
                 button_width, panel->height - panel->padding * 2, task->name);

  gui_button_task->meta_id = task->window;

  gui_add_child(panel->gui_container_tasks, gui_button_task);

  panel->tasks_count++;
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
  Panel *p = malloc(sizeof(Panel));
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
  Task *panel_task;

  bool found = false;

  while (task != NULL) {
    found = false;
    panel_task = panel->tasks;

    while (panel_task != NULL) {
      if (panel_task->window == task->window) {
        found = true;

        break;
      }

      panel_task = panel_task->next;
    }

    if (!found) {
      _add_task(panel, task);
    }

    task = task->next;
  }

  while (panel_task != NULL) {
    found = false;
    task = tasks;

    while (task != NULL) {
      if (panel_task->window == task->window) {
        found = true;

        break;
      }

      task = task->next;
    }

    if (!found) {
      _remove_task(panel, panel_task);

      break;
    }

    panel_task = panel_task->next;
  }
}

void panel_draw_gui(Panel *panel) {
  gui_draw_element(gui, panel->gui_container_tasks);
}
