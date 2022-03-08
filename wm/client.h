#pragma once

#include "window.h"

#include "stdbool.h"

/** 
 * Client is refering to a client window by abstraction,
 * being used in things like dock bar 
 *
 * For phisicaly client window, see Window
 */
typedef struct Client Client;
struct Client {
    Window *window;
    int float_x, float_y, float_width, float_height;
    bool has_float_cords;
    bool minimalized;
    bool maximalized;
    Client *next;
};

Client *client_create(Window *window);


