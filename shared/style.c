#include "style.h"

#include <stdlib.h>

Style *style_create() {
  Style *style = malloc(sizeof(Style));

  return style;
}
