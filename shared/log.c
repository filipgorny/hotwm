#include "log.h"

#include <stdio.h>

int debug_index = 0;

void log_error(char *message) { printf("[ERROR!] %s\n", message); }

void log_info(char *tag, char *message) { printf("[%s] %s\n", tag, message); }

void log_debug() {
  debug_index++;

  printf("\n*** DEBUG THICK (%d) ***\n\n", debug_index);
}

void log_debugv(char *v) {
  debug_index++;

  printf("\n*** DEBUG THICK (%d, %s) ***\n\n", debug_index, v);
}

void log_debugi(int i) {
  debug_index++;

  printf("\n*** DEBUG THICK (%d, %d) ***\n\n", debug_index, i);
}
