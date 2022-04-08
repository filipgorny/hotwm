#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "arg.h"

typedef struct {
} Controller;

Controller *controller_create();
void controller_run(Controller *controller, char* cmd, Arg arg);

#ifdef __cplusplus
}
#endif

