#pragma once

#include "window.h"

typedef struct Client Client;
struct Client {
    Window *window;
    Client *next;
};

Client *client_create(Window *window);

