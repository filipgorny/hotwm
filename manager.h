#include <xcb/xcb.h>

typedef struct {
    xcb_window_t window;
    int width;
    int height;
    int x;
    int y;
} Manager_window;

typedef struct {
    int window_count;
    Manager_window* window[];
} Manager_session;

xcb_connection_t *conn;

Manager_session* manager_start_session() {
    Manager_session *ms = malloc(sizeof(Manager_session));
    ms->window_count = 0;
    return ms;
}

Manager_window* manager_create_window(Manager_session* ms, xcb_window_t window) {
    Manager_window *mw = malloc(sizeof(Manager_window));
    mw->window = window;
    mw->width = 500;
    mw->height = 300;
    mw->x = 30;
    mw->y = 20;

    ms->window[ms->window_count++] = mw;

    return mw;
}

void manager_move_window(Manager_window *window, int x, int y) {
    window->x = x;
    window->y = y;
    manager_window_update(window);
}

void manager_window_update(Manager_window *window) {
  int vals[4];

  vals[0] = window->x;
  vals[1] = window->y;
  vals[2] = window->width;
  vals[3] = window->height;
  vals[4] = 2;
  xcb_configure_window(conn, window->window,
                       XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                           XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
                           XCB_CONFIG_WINDOW_BORDER_WIDTH,
                       vals);

  vals[0] = 0xFFFFFF;
  xcb_change_window_attributes(conn, window, XCB_CW_BORDER_PIXEL, vals);

  xcb_flush(conn);
}
