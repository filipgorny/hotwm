#include "gui.h"
#include <stdlib.h>

Gui *gui_initialize(Draw *draw) {
  Gui *gui = malloc(sizeof(Gui));
  gui->draw = draw;

  return gui;
}
