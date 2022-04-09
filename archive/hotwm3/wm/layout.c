#include "layout.h"
#include "window.h"

#include <stdlib.h>

Layout *layout_create(char *name, layout_callback) {
  Layout *layout = malloc(sizeof(Layout));
  layout->name = name;
  layout->apply = apply;

  return layout;
}

void layout_define_method(Layout *layout, layout_callback) {
  layout->apply = apply;
}
