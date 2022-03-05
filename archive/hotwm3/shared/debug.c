#include <stdio.h>

void debug_info(char *category, char *str, void *value) {
  char *info = sprintf(str, value);

  printf("[%s] %s", category, info);
}
