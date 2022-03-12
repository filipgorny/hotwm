#pragma once

#include <stdbool.h>

enum pointer_state {
    POINTER_STATE_FREE,
    POINTER_STATE_BUTTON1_DOWN,
    POINTER_STATE_BUTTON2_DOWN,
    POINTER_STATE_BUTTON3_DOWN,
    POINTER_STATE_BUTTON1_DOWN_META,
    POINTER_STATE_BUTTON2_DOWN_META,
    POINTER_STATE_BUTTON3_DOWN_META,
};


typedef struct {
    int x, y;
    enum pointer_state state;
} PointerState;

typedef struct {
    int x, y;
    enum pointer_state state;
} PointerStateChange;

typedef struct {
    PointerState *state;
    PointerStateChange *last_change;
} Pointer;

Pointer *pointer_initialize();
void pointer_update_cords(Pointer *pointer, int x, int y);
void pointer_update_state(Pointer *pointer, bool button1, bool button2, bool button3, bool meta);

