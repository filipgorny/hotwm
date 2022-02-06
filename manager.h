/*
typedef struct hot_window {
    xcb_window_t window;
} hot_window;

typedef struct hot_screen {
    xcb_screen_t *screen;
    xcb_window_t root;
}

typedef struct hot_manager 
    xcb_connection_t *connection;
    hot_screen* screens[];
    hot_window *windows;
    int window_count;
} hot_manager;

hot_manager* hot_create_manager(xcb_connection_t *conn, xcb_screen_t *screen){
    manager = malloc(sizeof(struct hot_manager));
    manager->connection = conn;
    manager->screen = screen;
    manager->windows = NULL;
    manager->window_count = 0;

    return manager;
}

void hot_move(hot_manager *manager, hot_window *window, int x, int y) {
    xcb_configure_window(manager->connection, window->window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, (uint32_t[]){x, y});
}
*/


