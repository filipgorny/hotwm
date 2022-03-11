#pragma once

#include "arg.h"

#define ACTION_METHOD void (*method)(Arg *)

typedef struct Action Action;
struct Action {
    char* name;
    ACTION_METHOD;
    Arg *args;
};

typedef struct {
    int actions_length;
    Action* actions[];
} ActionsRegistry;


ActionsRegistry *action_create_registry();

Action * action_create();

void action_define(ActionsRegistry *registry, char* name, ACTION_METHOD);
void action_execute(ActionsRegistry *registry, char* name);
