#include "spawn.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void spawn(const char *process) {
  printf("[SPAWN] Spawning %s\n", process);

  int status;

  if (fork() == 0) {
    status = system(process);
    exit(0);
  }
}
