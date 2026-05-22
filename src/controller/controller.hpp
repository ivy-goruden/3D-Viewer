#include "../include/globals.h"
#include "../include/figure/figure.hpp"
#include "../include/grid.hpp"

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
            int shiftx_;
            int shifty_;
            int diagonal_;

            Vert_t getFigureProjection(const matrix_t original, double minz);
        public:
            Controller();
            Vert_t loadFigure(const char* filename, int, int);
            Vert_t getFigure();            
            void toggleProjection();
            void shiftX(int);
            void shiftY(int);
            Edge_t getEdges();
            Poly_t getPolygons();
            double getAngleX();
            double getAngleY();
            double getAngleZ();
            void setAngleX(int angle);
            void setAngleY(int angle);
            void setAngleZ(int angle);
            void setScale(double scale);
            void setCamera(int camera);
            double getScale();
            int getZoom();
            int getEdgesNum();
            int getVerticesNum();
            Bounds getFigureBounds();


    };
}