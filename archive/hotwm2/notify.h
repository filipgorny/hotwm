#include "arg.h"

void notify(char* name, const Arg *arg);

#define __(s) notify(s, NULL)
#define ___(s, v) notify(s, v)
