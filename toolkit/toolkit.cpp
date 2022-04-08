#include "toolkit.hpp"

extern "C" {
    View toolkit_generate(GuiElement gui_element);
        View *view = (View*)malloc(sizeof(View));

        return *view;
    }
}
