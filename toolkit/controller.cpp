#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "controller.hpp"

using namespace controller;

void Controller::add_action(const char* name, void (*action)(void)) {
    ControllerAction* controller_action = (ControllerAction*)malloc(sizeof( ControllerAction));
    controller_action->name = name;
    controller_action->action = action;

    this->actions[this->actions_count] = controller_action;
    this->actions_count++;
}

void Controller::trigger_action(const char* name) {
    for (int i = 0; i < this->actions_count; i++) {
        if (strcmp(this->actions[i]->name, name) == 0) {
            this->actions[i]->action();
            return;
        }
    }
}
