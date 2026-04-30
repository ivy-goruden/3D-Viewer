#include "transformer.hpp"
#include <cmath>
namespace s21{

    Transformer::Transformer(){}
    Vert_t Transformer::getParallelProjection(matrix_t m, double camera){
        //returns a list of vertices in new position
        Vert_t projection;
        for(int i =0; i < m.size(); i++){
            Point coord = Point{m[i][0]/camera,m[i][1]/camera};
            projection.push_back(coord);
        }

        return projection;
    }

    Vert_t Transformer::getPerspectiveProjection(matrix_t m, double camera){
        //returns a list of vertices in new position
        Vert_t projection;
        for(int i =0; i < m.size(); i++){
            double z = m[i][2] + camera;
            if (z<= 0) continue; 
            Point coord = Point{m[i][0]/z,m[i][1]/z};
            projection.push_back(coord);
        }

        return projection;
    }

    matrix_t Transformer::Rotate(int angleX_deg, int angleY_deg, int angleZ_deg, const matrix_t& original){
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
        matrix_t result =  multiplyMatrix(&original, &x_rotate);
        result =  multiplyMatrix(&result, &y_rotate);
        result =  multiplyMatrix(&result, &z_rotate);
        return result;

    }

    matrix_t Transformer::multiplyMatrix(const matrix_t *f, const matrix_t *s){
        int rows = f->size();
        int cols = s[0].size();
        int inner = s->size();
        matrix_t nMatrix = createMatrix(rows, cols);
        for (int row = 0; row<rows;row++){
            for (int col = 0; col < cols; col++){
                double val = 0;
                for (int x = 0; x<inner;x++){
                    val += ((*f)[row][x])*((*s)[x][col]);
                }
                nMatrix[row][col] = val;
            }

        }
        return nMatrix;
    }

    matrix_t Transformer::createMatrix(int rows, int cols){
        std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0));
        return matrix;
    }

}