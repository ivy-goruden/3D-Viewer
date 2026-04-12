#ifndef DRAWER
#define DRAWER

#include "globals.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <utilities.h>
namespace s21{
class Drawer
{
    public:
        void draw(ProjectionData_t);
        void drawVert(Vert_t);
        void drawPoly(Vert_t, Poly_t);
        void srawNodes(Vert_t, Node_t);
        Drawer();
        ~Drawer();

    protected:
        double scale_;
        double xStart_;
        double yStart_;

}
}
#endif