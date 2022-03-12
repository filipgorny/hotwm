#pragma once
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

#include "controller.h"
#include "keymap.h"
#include "session.h"

typedef struct {
    Controller *controller;
    Keymap *keymap;
    Session *session;
} ScriptingContext;

ScriptingContext *scripting_create_context(Controller *controller, Keymap *keymap, Session *session);
void scripting_run(ScriptingContext *context, char *filename);
