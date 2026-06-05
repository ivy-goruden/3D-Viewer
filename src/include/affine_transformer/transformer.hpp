#ifndef TRANSFORMER
#define TRANSFORMER

#include "../globals.h"
#include "../camera/camera.hpp"
#include "../algebra/algebra.hpp"

namespace s21{
    
    //Класс афинных преобразований фигур
    class Transformer{
        public:
            Transformer() {};
            matrix_t static Translate(float tx, float ty, float tz, const matrix_t&);
            matrix_t static Rotate(int angleX, int angleY, int angleZ, const matrix_t&);
            matrix_t static Scale(float sx, float sy, float sz, const matrix_t&);
            Poly_Proj_t static getParallelProjection(Poly_t &polygons, const matrix_t &m, float scale);
            Poly_Proj_t static getPerspectiveProjection(Poly_t &polygons, const matrix_t &m, Camera& camera, 
            float scale);
            bool static PolyVisible(std::vector<int> &polygon, const matrix_t &m, Camera& camera);
            bool static PolyInvisible(std::vector<int> &polygon, const matrix_t &m, Camera& camera);
            Point3d static getPolyCenter(std::vector<Point3d> &polygon);
            Poly_Filled_t static cropFigure(Poly_t &polygons, const matrix_t &m, Camera& camera);
            std::vector<Point3d> static cropPoly(std::vector<int> &polygon, const matrix_t &m, Camera& camera);

            matrix_t static perspective(matrix_t* original, const Camera* camera, float aspect);
            matrix_t static ortho(matrix_t* original, const Camera* camera, float aspect);
            matrix_t static setView(matrix_t* original, const Camera* camera, const Point3d center);
    };
}

#endif
