#include "style.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Style *style_create() {
  Style *style = malloc(sizeof(Style));
  style->values_count = 0;

  return style;
}

void style_define(Style *style, const char *name, const int value) {
  StyleValue *style_value = malloc(sizeof(StyleValue));
  style_value->name = name;
  style_value->value = value;

  style->values[style->values_count] = style_value;

  style->values_count++;
}

int style_get(Style *style, const char *name) {
  for (int i = 0; i < style->values_count; i++) {
    if (style->values[i]->name != NULL &&
        strcmp(style->values[i]->name, name) == 0) {
      return style->values[i]->value;
    }
  }

  return 0;
}
