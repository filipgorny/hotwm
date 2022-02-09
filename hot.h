#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb_keysyms.h>

typedef union {
    int i;
    unsigned int ui;
    float f;
    char* v;
} Hot_arg;

typedef struct {
    unsigned int modifiers;
    xcb_keysym_t keysym;
    void (*func)(const Hot_arg *);
    const Hot_arg arg;
} Hot_key;


void hot_run(const Hot_arg *value) {
    int status;

    if(fork() == 0){ 
        status = system(value->v);
        exit(0);
    } 
}

