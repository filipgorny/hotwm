#pragma once

typedef struct {
    int x, y;
} GridPoint;

typedef struct {
    int resolution;
} Grid;

Grid *grid_create();
GridPoint *grid_get_nearest_point(Grid grid, int x, int y);
