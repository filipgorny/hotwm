#include "layout.h"

#ifndef ARG_H
#define ARG_H
typedef union {
  int i;
  unsigned int ui;
  float f;
  char *v;
  enum layout layout
} Arg;

#endif
