#pragma once

#include "layout.h"

typedef struct {
    Layout *layout;
} Screen;

void screen_refresh();
