#pragma once

#include "arg.h"

typedef struct {
} Controller;

Controller *controller_create();
void controller_run(Controller *controller, char* cmd, Arg arg);
