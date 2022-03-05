#pragma once

#include "draw.h"

#define color u_int32_t

typedef struct {
    int x, y;
    int width, height;
    color background;
    color foreground;
} Params;

typedef struct {
    char *name;
    void (*draw_func)(Draw *draw, Params *params);
} Type;

typedef struct Definition Definition;
struct Definition {
    Type *type;
    Definition *next;
};

typedef struct {
    int x, y;
    int width, height;
    Type *type;
} Element;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Container;

typedef struct {
    Draw *draw;
    Definition *definitions;
} Gui;

Gui *gui_initialize(Draw *draw);

