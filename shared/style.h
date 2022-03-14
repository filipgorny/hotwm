#pragma once

#define COLOR u_int32_t

#include <sys/types.h>

typedef struct {
    const char* name;
    int value;
} StyleValue;

typedef struct {
    int gap;
    int margin;
    COLOR title_bar_color;
    COLOR window_border_color;
    COLOR title_bar_text_color;
    int title_bar_height;
    int window_padding;
    int title_bar_margin;
    int title_bar_text_padding_bottom;
    int title_bar_text_padding_left;
} StyleOld;

typedef struct {
    int values_count;
    StyleValue* values[];
} Style;

Style *style_create();
void style_define(Style *style, const char *name, const int value);
int style_get(Style *style, const char *name);
