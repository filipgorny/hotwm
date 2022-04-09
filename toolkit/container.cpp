#include "container.hpp"

using namespace component;

void Container::add_child(Component* child) {
    this->children = (Component**)realloc(this->children, sizeof(Component*) * (this->children_count + 1));
    this->children[this->children_count] = child;
    this->children_count++;
}

Component** Container::get_children() {
    return this->children;
}
