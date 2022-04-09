#pragma once

#include <stdlib.h>

#include "component.hpp"
#include "definition.hpp"

namespace view {
    struct View {
        component::Component *root;
    };


    typedef View View;
}

#include "view.hpp"

