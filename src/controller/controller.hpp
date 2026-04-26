#include "../include/globals.h"
#include "../include/figure/figure.hpp"
namespace s21{

    class Controller{
        protected:
            Figure *figure_ = nullptr;
            bool parallel_projection_ = false;
            double angleX_;
            double angleY_;
            double angleZ_;

            Vert_t getFigureProjection(const matrix_t original);
            Vert_t rotateFigure(double x, double y, double z);
        public:
            Controller();
            Vert_t loadFigure(const char* filename);
            Vert_t rotateAbsolute(double x, double y, double z);
            Vert_t rotateX(double);
            Vert_t rotateY(double);
            Vert_t rotateZ(double);
            Vert_t toggleProjection();

            Edge_t getEdges();
            Poly_t getPolygons();
            double getAngleX();
            double getAngleY();
            double getAngleZ();
            // moveFigure();
            // scaleFigure();
    };
}