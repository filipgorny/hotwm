#include "controller.h"
#include "arg.h"
#include "spawn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Controller *controller_create() {
  Controller *controller = (Controller *)malloc(sizeof(Controller));

  return controller;
}

void controller_run(Controller *controller, char *cmd, Arg arg) {
  printf("[Controller] CMD: '%s'\n", cmd);

  if (strcmp(cmd, "spawn") == 0) {
    //    spawn(arg.v);
  }
}
