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
    Layout layouts[];
} Session;

Session *session_start(xcb_screen_t *screen);
void session_add_client(Session *session, Client *client);
void session_add_layout(Session *session, Layout *layout);
Layout *session_current_layout(Session *session);
