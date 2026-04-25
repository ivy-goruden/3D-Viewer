#include "../include/globals.h"
#include "../include/figure/figure.hpp"
namespace s21{

    class Controller{
        protected:
            Figure *figure_ = nullptr;
            bool parallel_projection_ = false;

            Vert_t getFigureProjection(const matrix_t original);
        public:
            Controller();
            Vert_t loadFigure(const char* filename);
            Vert_t rotateFigure(double x, double y, double z);
            Vert_t toggleProjection();
            // moveFigure();
            // scaleFigure();
    };
}