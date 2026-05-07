#include "grid.hpp"
#include "globals.h"

namespace s21 {
    Grid::Grid() {
        m = matrix_t();
    }

    Grid::~Grid() {
        //
    }

    void Grid::createGrid(Bounds bounds, double step) {
        m = matrix_t();
        fillGrid(bounds.minx, bounds.maxx, bounds.miny, bounds.maxy, bounds.minz, bounds.maxz, step);
    }

    void Grid::createGrid(double x1, double x2, double y1, double y2, double z1, double z2, double step) {
        m = matrix_t();
        fillGrid(x1, x2, y1, y2, z1, z2, step);
    }

    void Grid::createGrid(s21::Figure& figure, double step) {
        m = matrix_t();
        Bounds b = figure.getBounds();
        fillGrid(b.minx, b.maxx, b.miny, b.maxy, b.minz, b.maxz, step);
    }

    void Grid::fillGrid(double x1, double x2, double y1, double y2, double z1, double z2, double step) {
        double dx = x2 - x1;
        double dy = y2 - y1;
        double dz = z2 - z1;
        double x = x1 - dx;
        double y = y1 - dy;
        double z = z1 - dz;
        x2 += dx;
        y2 += dy;
        z2 += dz;    
        while (x < x2) {
            while (y < y2) {
                while (z < z2) {
                    m.push_back({x, y, z, 1});
                    z += step;
                }
                y += step;
            }
            x += step;
        }
    }

    matrix_t Grid::getMatrix() {
        return m;
    }

}
