#pragma once

#include "window.h"

/** 
 * Client is refering to a client window by abstraction,
 * being used in things like dock bar 
 *
 * For phisicaly client window, see Window
 */
typedef struct Client Client;
struct Client {
    Window *window;
    int float_x, float_y;
    Client *next;
};

Client *client_create(Window *window);

