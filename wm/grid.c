#include "grid.h"

#include <stdlib.h>

Grid *grid_create() {
  Grid *grid = malloc(sizeof(Grid));

  return grid;
}

GridPoint *grid_get_nearest_point(Grid grid, int x, int y) {
  GridPoint *point = malloc(sizeof(GridPoint));

  return point;
}
