#include "transformer.hpp"
#include "matrix.hpp"
#include <cmath>
#include <format>
#include <string>
#include <iostream>

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
            double z = m[i][2]+camera;
            if (z<=(-minz)) {
                continue;
            }
            double factor = 1.0 / z;
            Point coord = Point{m[i][0]*factor*scale,m[i][1]*factor*scale};
            projection.push_back(coord);
        }
        return projection;
    }

    matrix_t Transformer::Rotate(int angleX_deg, int angleY_deg, int angleZ_deg, const matrix_t& original) {
        if (original.empty()){
            return matrix_t();
        }
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
        matrix_t result = Matrix::multiplyMatrix(&original, &x_rotate);
        result = Matrix::multiplyMatrix(&result, &y_rotate);
        result = Matrix::multiplyMatrix(&result, &z_rotate);
        return result;
    }

    matrix_t Transformer::Translate(double tx, double ty, double tz, const matrix_t& original) {
        if (original.empty()){
            return matrix_t();
        }
        const matrix_t translate = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {tx, ty, tz, 1}
        };
        return Matrix::multiplyMatrix(&original, &translate);
    }

    matrix_t Transformer::Scale(double sx, double sy, double sz, const matrix_t& original) {
        if (original.empty()){
            return matrix_t();
        }
        const matrix_t sc = {
            {sx, 0, 0, 0},
            {0, sy, 0, 0},
            {0, 0, sz, 0},
            {0, 0, 0, 1}
        };
        return Matrix::multiplyMatrix(&original, &sc);
    }

}
