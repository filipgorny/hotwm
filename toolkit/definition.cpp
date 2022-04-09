#include "definition.hpp"

using namespace definition;

ComponentDefinition::ComponentDefinition(int type) {
    this->type = type;
}

void ComponentDefinition::define(std::string name, PropertyDefValue value) {
    Property* property = (Property*) malloc(sizeof(Property));

    this->properties[this->property_count] = property;
    this->property_count++;
}

PropertyDefValue* ComponentDefinition::get(std::string name) {
    for (int i = 0; i < this->property_count; i++) {
        if (this->properties[i]->name == name) {
            return &this->properties[i]->value;
        }
    }

    return NULL;
}
