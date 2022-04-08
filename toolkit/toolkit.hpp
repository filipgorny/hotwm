#pragma once

#include "gui.h"
#include "view.hpp"
#include "definition.hpp"

using View = ui::View;

extern "C" {
    typedef View View;
    typedef component::ViewDef ViewDef;

    View toolkit_generate(GuiElement gui_element);
}

