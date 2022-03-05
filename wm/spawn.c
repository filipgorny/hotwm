#include "spawn.h"
#include <stdlib.h>
#include <unistd.h>

void spawn(Arg *arg) {
  printf("[SPAWN] Spawning %s\n", arg->v);

  int status;

  if (fork() == 0) {
    status = system(arg->v);
    exit(0);
  }
}
