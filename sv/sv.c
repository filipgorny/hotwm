#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void spawn(char *cmd) {
  int status;

  if (fork() == 0) {
    status = system(cmd);
    exit(0);
  }
}

int main() {
  spawn("./hotwm");
  spawn("./hotshell");
}
