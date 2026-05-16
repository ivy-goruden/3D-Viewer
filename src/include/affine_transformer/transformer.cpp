#include "transformer.hpp"
#include "matrix.hpp"
#include <cmath>

namespace s21{

    Vert_t Transformer::getParallelProjection(matrix_t m, double scale){
        //returns a list of vertices in new position
        Vert_t projection;
        for(int i =0; i < m.size(); i++){
            Point coord = Point{m[i][0]*scale,m[i][1]*scale};
            projection.push_back(coord);
        }
        return projection;
    }

    Vert_t Transformer::getPerspectiveProjection(matrix_t m, double camera, 
            double scale, double minz) {
        //returns a list of vertices in new position
        minz = minz < 0 ? minz : 0;
        Vert_t projection;        
        for (int i = 0; i < m.size(); i++) {
            double z = m[i][2] + camera - minz;
            if (z <= 0) {
                continue;
            }
            Point coord = Point{m[i][0]/z*scale,m[i][1]/z*scale};
            projection.push_back(coord);
        }
        return projection;
    }

    matrix_t Transformer::Rotate(int angleX_deg, int angleY_deg, int angleZ_deg, const matrix_t& original) {
        double angleX = angleX_deg * M_PI / 180.0;
        double angleY = angleY_deg * M_PI / 180.0;
        double angleZ = angleZ_deg * M_PI / 180.0;
        const matrix_t x_rotate = {
            {1,0,0,0},
            {0, std::cos(angleX), std::sin(angleX)*-1,0},
            {0, std::sin(angleX),std::cos(angleX), 0},
            {0,0,0,1}
        };
        const matrix_t y_rotate = {
            {std::cos(angleY), 0, std::sin(angleY), 0},
            {0,1,0,0},
            {std::sin(angleY)*-1, 0, std::cos(angleY), 0},
            {0,0,0,1}
        };
        const matrix_t z_rotate = {
            {std::cos(angleZ), std::sin(angleZ)*-1, 0, 0},
            {std::sin(angleZ),std::cos(angleZ),0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
        matrix_t result =  Matrix::multiplyMatrix(&original, &x_rotate);
        result =  Matrix::multiplyMatrix(&result, &y_rotate);
        result =  Matrix::multiplyMatrix(&result, &z_rotate);
        return result;
    }

    matrix_t Transformer::Translate(double cx, double cy, double cz, const matrix_t& original) {
        const matrix_t translate = {
            {1, 0, 0, cx},
            {0, 1, 0, cy},
            {0, 0, 1, cz},
            {0, 0, 0, 1}
        };
        matrix_t result;
        for (int i = 0; i < original.size(); i++) {
            result.push_back({original[i][0] + cx, original[i][1] + cy, original[i][2] + cz});
        }
        return result;
    }

    Bounds Transformer::getBounds(matrix_t& matrix_) {
        Bounds bounds = {
            matrix_[0][0], matrix_[0][0],
            matrix_[0][1], matrix_[0][1],
            matrix_[0][2], matrix_[0][2]
        };
        for (int i = 0; i < matrix_.size(); i++) {
            if (bounds.maxx < matrix_[i][0]) bounds.maxx = matrix_[i][0];
            if (bounds.minx > matrix_[i][0]) bounds.minx = matrix_[i][0];
            if (bounds.maxy < matrix_[i][1]) bounds.maxy = matrix_[i][1];
            if (bounds.miny > matrix_[i][1]) bounds.miny = matrix_[i][1];
            if (bounds.maxz < matrix_[i][2]) bounds.maxz = matrix_[i][2];
            if (bounds.minz > matrix_[i][2]) bounds.minz = matrix_[i][2];
        }
        return bounds;
    }
}
