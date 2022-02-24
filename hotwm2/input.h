#include "arg.h"
#include <xcb/xcb.h>

#ifndef INPUT_H
#define INPUT_H
typedef struct {
  unsigned int click;
  unsigned int mask;
  unsigned int button;
  void (*func)(const Arg *arg);
  const Arg arg;
} Button;

typedef struct {
  unsigned int modifiers;
  xcb_keysym_t keysym;
  void (*func)(const Arg *);
  const Arg arg;
} Key;

typedef struct {
  int x, y;
} Mouse;

#endif
