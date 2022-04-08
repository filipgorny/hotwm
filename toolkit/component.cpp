#include <stdlib.h>
#include <stdio.h>

#include "../shared/gui.h"

#include "component.hpp"

using namespace component;

void voidMethod() {};

Component::Component() {
    this->style = (Style*) malloc(sizeof(Style));
    this->onClickCallback = voidMethod;
    this->onHoverCallback = voidMethod;
}

void Component::init() {
}

void Component::onClick() {
    this->onClickCallback();
}

void Component::onHover() {
    this->onHoverCallback();
}

void Component::setOnClick(Callback callback) {
    this->onClickCallback = callback;
}

void Component::setOnHover(Callback callback) {
    this->onHoverCallback = callback;
}

