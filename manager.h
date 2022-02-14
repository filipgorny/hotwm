typedef union {
    int i;
    unsigned int ui;
    float f;
    char* v;
} Manager_arg;

typedef struct {
    unsigned int modifiers;
    xcb_keysym_t keysym;
    void (*func)(const Manager_arg *);
    const Manager_arg arg;
} Manager_key;

typedef struct {
    int stack_width_percent;
    int gap_width;
    int border_width;
    int border_color_active;
    int border_color_inactive;
    char* current_layout;
    Manager_key     keys[];
} Manager_setting;

typedef struct {
    int id;
    xcb_window_t window;
    int width;
    int height;
    int x;
    int y;
} Manager_window;

typedef struct {
    xcb_connection_t *conn;
    xcb_screen_t *screen;
    int window_count;
    int active_window;
    int selected_window_id;
    Manager_setting* setting;
    Manager_window* window[];
} Manager_session;

typedef struct {
    char* name;
    char* symbol;
    void (*init)();
} Manager_layout;

Manager_setting *manager_setting_create();
Manager_session* manager_start_session(xcb_connection_t *conn, xcb_screen_t *screen, Manager_setting *setting);
Manager_window* manager_create_window(Manager_session* ms, xcb_window_t window);
void manager_move_window(Manager_session* manager_session, Manager_window *window, int x, int y);
void manager_window_update(Manager_session* manager_session, Manager_window *window); 
void manager_trigger_key(Manager_session* manager_session, xcb_keysym_t keysym, uint16_t state);
void manager_layout_update(Manager_session *manager_session);
void manager_run(const Manager_arg *arg);
