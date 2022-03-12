#include "spawn.h"
#include <stdlib.h>
#include <unistd.h>

void spawn(char *process) {
  printf("[SPAWN] Spawning %s\n", process);

  int status;

  if (fork() == 0) {
    status = system(process);
    exit(0);
  }
}
