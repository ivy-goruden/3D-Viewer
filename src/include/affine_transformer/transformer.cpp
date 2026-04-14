#include "transformer.hpp"
#include <cmath>
namespace s21{

    Transformer::Transformer(){}
    Vert_t Transformer::getFigureProjection(matrix_t m){
        //returns a list of vertices in new position
        Vert_t projection = {};
        for(int i =0; i < m.size(); i++){
            Point coord = Point{m[i][0],m[i][1]};
            projection.push_back(coord);
        }
        return projection;
    }

    matrix_t Transformer::Rotate(int angleX, int angleY, int angleZ, matrix_t original){
        matrix_t x_rotate = {
            {1,0,0,0},
            {0, std::cos(angleX), std::sin(angleX)*-1,0},
            {0, std::sin(angleX),std::cos(angleX), 0},
            {0,0,0,1}
        };
        matrix_t y_rotate = {
            {std::cos(angleY), 0, std::sin(angleY), 0},
            {0,1,0,0},
            {std::sin(angleY)*-1, 0, std::cos(angleY), 0},
            {0,0,0,1}
        };
        matrix_t z_rotate = {
            {std::cos(angleZ), std::sin(angleY)*-1, 0, 0},
            {std::sin(angleZ),std::cos(angleZ),0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
        matrix_t result =  multiplyMatrix(&original, &x_rotate);
        result =  multiplyMatrix(&result, &y_rotate);
        result =  multiplyMatrix(&result, &z_rotate);
        return result;

    }

    matrix_t Transformer::multiplyMatrix(matrix_t *f, matrix_t *s){
        int rows = f->size();
        int cols = s->size();
        matrix_t nMatrix = createMatrix(rows, cols);
        for (int row = 0; row<rows;row++){
            for (int col = 0; col < cols; col++){
                int val = 0;
                for (int y = 0; y < cols; y++){
                    for (int x = 0; x<rows;x++){
                        val += ((*f)[row][x])*((*s)[y][col]);
                    }
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