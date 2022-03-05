#pragma once

#include "client.h"
#include "window.h"

typedef struct Desktop Desktop;
struct Desktop {
    Window *window;
    Desktop *prev;
    Desktop *next;
    Client *clients; 
};

typedef struct {
    Desktop *desktops;
    Desktop *current_desktop;
} Session;

Session *session_start();
void session_add_client(Session *session, Client *client);
