#include "input.h"
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#define MODKEY Mod1Mask

xcb_connection_t *conn;

InputConfig *input_config;

void spawn(const Arg *arg) {
  int status;

  if (fork() == 0) {
    status = system(arg->v);
    exit(0);
  }
}

int main() {
  conn = xcb_connect(NULL, NULL);
  input_config = input_create_config(conn);

  Arg arg = {.v = "/bin/st"};
  input_define_key(input_config, XK_C, MODKEY, spawn, arg);
}
