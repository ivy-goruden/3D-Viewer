#include "../include/globals.h"
#include "../include/figure/figure.hpp"
namespace s21{

    class Controller{
        protected:
            Figure figure_;
        public:
            Controller(Figure);
            Vert_t loadFigure(const char* filename);
            Vert_t rotateFigure(double x, double y, double z);
            // moveFigure();
            // scaleFigure();
    };
}