#include "action.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

ActionsRegistry *action_create_registry() {
  ActionsRegistry *a = malloc(sizeof(ActionsRegistry));

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

  Action *action = registry->actions;

  if (!action) {
    registry->actions = a;

    return;
  }

  while (action) {
    if (!action->next) {
      action->next = a;

      return;
    }

    action = action->next;
  }
}

void action_execute(ActionsRegistry *registry, char *name) {
  Action *a = registry->actions;

  while (a) {
    printf("Testing action %s\n", a->name);
    if (strcmp(a->name, name) == 0) {
      log_info("action", "Found and executing action");
      log_info("action", a->name);

      a->method(a->args);

      return;
    }

    a = a->next;
  }

  log_info("action", "No action definition found");
}
