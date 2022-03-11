#include "action.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ActionsRegistry *action_create_registry() {
  ActionsRegistry *a = malloc(sizeof(ActionsRegistry));
  a->actions_length = 0;

  return a;
}

Action *action_create() {
  Action *a = malloc(sizeof(Action));
  a->name = "";
  a->method = NULL;

  return a;
}

void action_define(ActionsRegistry *registry, char *name, void (*func)()) {
  Action *a = action_create();
  a->name = name;
  a->method = func;

  registry->actions[registry->actions_length] = a;
  registry->actions_length++;

  printf("Dodano akcje, ilosc: %d\n", registry->actions_length);
}

void action_execute(ActionsRegistry *registry, char *name) {
  int i;

  printf("Count actions: %d\n", registry->actions_length);

  for (i = 0; i < registry->actions_length; i++) {
    printf("Testing %s\n", registry->actions[i]->name);
    if (registry->actions[i]->name == name) {
      log_info("action", "Firing action.");
      registry->actions[i]->method(registry->actions[i]->args);

      return;
    }
  }

  log_info("action", "No action definition found");
}
