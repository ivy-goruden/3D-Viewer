#ifndef GRID_HPP
#define GRID_HPP

#include "globals.h"

namespace s21 {

class Grid {
   private:
    matrix_t m;

   public:
    Grid();
    ~Grid();
    void createGrid(Bounds bounds, float step);
    void createGrid(float x1, float x2, float y1, float y2, float z1, float z2, float step);
    void fillGrid(float x1, float x2, float y1, float y2, float z1, float z2, float step);
    matrix_t getMatrix();
};

}  // namespace s21

#endif
