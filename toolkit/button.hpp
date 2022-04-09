#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "component.hpp"
#include "definition.hpp"

namespace component {
    class Button: public Component {
        private:
        public:
            std::string label;
            static Component* create(definition::ComponentDefinition* def) {
                Button* button = new Button();
                button->label = def->get("label");

                return button;
            }
    };
};
