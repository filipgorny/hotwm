#pragma once

#define COLOR int

#define NUMBER int

#include <sys/types.h>

typedef struct {
    const char* name;
    int value;
} StyleValue;

typedef struct {
    NUMBER gap;
    NUMBER margin;
    COLOR title_bar_color;
    COLOR window_border_color;
    COLOR title_bar_text_color;
    NUMBER title_bar_height;
    NUMBER window_padding;
    NUMBER title_bar_margin;
    NUMBER title_bar_text_padding_bottom;
    NUMBER title_bar_text_padding_left;
} StyleOld;

typedef struct {
    int values_count;
    StyleValue* values[];
} Style;

Style *style_create();
void style_define(Style *style, const char *name, const int value);
int style_get(Style *style, const char *name);
