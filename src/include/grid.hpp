#ifndef GRID_HPP
#define GRID_HPP

#include "globals.h"
#include "figure/figure.hpp"

namespace s21 {

    class Grid {
    private:
        matrix_t m;

    public:
        Grid();
        ~Grid();
        void createGrid(Bounds bounds, double step);
        void createGrid(double x1, double x2, double y1, double y2, double z1, double z2, double step);
        void createGrid(Figure& figure, double step);
        void fillGrid(double x1, double x2, double y1, double y2, double z1, double z2, double step);
        matrix_t getMatrix();
    };

}

#endif
