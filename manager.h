#include <xcb/xcb.h>
#include "setting.h"

typedef struct {
    int id;
    xcb_window_t window;
    int width;
    int height;
    int x;
    int y;
} Manager_window;

typedef struct {
    int window_count;
    int active_window;
    int selected_window_id;
    Setting* setting;
    Manager_window* window[];
} Manager_session;

xcb_connection_t *conn;
xcb_screen_t *screen;
Manager_session* manager_session;

Manager_session* manager_start_session() {
    Manager_session *ms = malloc(sizeof(Manager_session));
    ms->window_count = 0;
    ms->active_window = 0;
    ms->setting = setting_create();
    return ms;
}

Manager_window* manager_create_window(xcb_window_t window) {
    Manager_window *mw = malloc(sizeof(Manager_window));
    mw->id = mw->id++;
    mw->window = window;
    mw->width = 500;
    mw->height = 300;
    mw->x = 30;
    mw->y = 20;

    manager_session->window[manager_session->window_count++] = mw;

    manager_session->active_window = mw->id;

    manager_window_update(mw);
    
    manager_layout_windows_stack();

    return mw;
}

void manager_move_window(Manager_window *window, int x, int y) {
    window->x = x;
    window->y = y;
    manager_window_update(window);
}

void manager_window_update(Manager_window *window) {
  int vals[5];

  vals[0] = window->x + manager_session->setting->gap_width;
  vals[1] = window->y + manager_session->setting->gap_width;
  vals[2] = window->width - manager_session->setting->gap_width;
  vals[3] = window->height - manager_session->setting->gap_width;
  vals[4] = 2;
  xcb_configure_window(conn, window->window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
                           XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       vals);

  if (window->id == manager_session->active_window) {
    vals[0] = manager_session->setting->border_color_active;
  } else {
    vals[0] = manager_session->setting->border_color_inactive;
  }

  xcb_change_window_attributes(conn, window->window, XCB_CW_BORDER_PIXEL, vals);

  xcb_flush(conn);
}

void manager_layout_windows_stack() {
    int main_x = 0;
    int main_y = 0;
    int main_width = screen->width_in_pixels;
    int main_height = screen->height_in_pixels;

    if (manager_session->window_count > 1) {
        //main_x = manager_session->setting->stack_width_percent / 100 * screen->width_in_pixels;
        
        main_x = 400;
        main_width = screen->width_in_pixels - main_x;
    }

    int stack_window_index = 0;
    int i;

    for (i = 0; i < manager_session->window_count; i++) {
        Manager_window *window = manager_session->window[i];
        if (window->id == manager_session->active_window) {
            window->x = main_x;
            window->y = main_y;
            window->width = main_width;
            window->height = main_height;
        } else {
            window->x = 0;
            window->y = (screen->height_in_pixels / (manager_session->window_count - 1) + 0.5f) * stack_window_index;
            window->width = screen->width_in_pixels - main_width;
            window->height = screen->height_in_pixels / (manager_session->window_count - 1);

            stack_window_index++;
        }

        manager_window_update(window);
    }
}
