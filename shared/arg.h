#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
  int i;
  char *v;
  int x, y;
  void (*f)(); 
} Arg;

#ifdef __cplusplus
}
#endif
