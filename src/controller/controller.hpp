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
            double camera_;
            double scale_;

            Vert_t getFigureProjection(const matrix_t original, double minz);
        public:
            Controller();
            Vert_t loadFigure(const char* filename);
            Vert_t getFigure();
            
            void toggleProjection();
            Edge_t getEdges();
            Poly_t getPolygons();
            double getAngleX();
            double getAngleY();
            double getAngleZ();
            void setAngleX(int angle);
            void setAngleY(int angle);
            void setAngleZ(int angle);
            void setScale(double scale);
            int getEdgesNum();
            int getVerticesNum();
    };
}