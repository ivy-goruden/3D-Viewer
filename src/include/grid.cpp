#include "grid.hpp"

#include "globals.h"

namespace s21 {
Grid::Grid() { m = matrix_t(); }

Grid::~Grid() {
    //
}

void Grid::createGrid(Bounds bounds, float step) {
    m = matrix_t();
    fillGrid(bounds.minx, bounds.maxx, bounds.miny, bounds.maxy, bounds.minz, bounds.maxz, step);
}

void Grid::createGrid(float x1, float x2, float y1, float y2, float z1, float z2, float step) {
    m = matrix_t();
    fillGrid(x1, x2, y1, y2, z1, z2, step);
}

void Grid::fillGrid(float x1, float x2, float y1, float y2, float z1, float z2, float step) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    float x = x1 - dx;
    float y = y1 - dy;
    float z = z1 - dz;
    x2 += dx;
    y2 += dy;
    z2 += dz;
    while (x < x2) {
        while (z < z2) {
            m.push_back({x, 0, z, 1});
            z += step;
        }
        z = z1 - dz;
        x += step;
    }
}

matrix_t Grid::getMatrix() { return m; }

}  // namespace s21
