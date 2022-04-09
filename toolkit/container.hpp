#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "component.hpp"

namespace component {
     class Container: public Component {
        private:
            Component** children;
            int children_count;
        public:
            void add_child(Component* child);
            Component** get_children();
    };
};
