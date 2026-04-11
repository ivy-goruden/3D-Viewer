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
        void draw(matrix_t);
        Drawer();
        ~Drawer();

    protected:
        double scale;
        double X_START;
        double Y_START;

}
}
#endif