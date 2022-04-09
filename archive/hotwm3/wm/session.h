#pragma once

#include "client.h"
#include "layout.h"
#include "window.h"

typedef struct Desktop Desktop;
struct Desktop {
    Window *window;
    Desktop *prev;
    Desktop *next;
    Client *clients; 
    Client *current_client;
    xcb_screen_t *screen;
};

typedef struct {
    Desktop *desktops;
    Desktop *current_desktop;
    int layout_count;
    int current_layout_index;
    Layout* layouts[];
} Session;

Session *session_start(xcb_screen_t *screen);
void session_add_client(Session *session, Client *client);
void session_add_layout(Session *session, Layout *layout);
Layout *session_current_layout(Session *session);
Client *session_find_client_by_xcb_window(Session *session, xcb_window_t window);

void session_select_next_client(Session *session);
void session_select_previous_client(Session *session);

Layout *session_get_layout(Session *session, char* name);

int session_desktop_client_count(Desktop *desktop);
void session_select_layout(Session *session, char* name);
