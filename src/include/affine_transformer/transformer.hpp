#ifndef TRANSFORMER
#define TRANSFORMER
#include "../globals.h"
namespace s21{
    
    //Класс афинных преобразований фигур
    class Transformer{
        public:
            Transformer();
            matrix_t static Rotate(int angleX, int angleY, int angleZ, matrix_t);
            Vert_t static getFigureProjection(matrix_t);
            matrix_t static multiplyMatrix(matrix_t *f, matrix_t *s);
            matrix_t static createMatrix(int rows, int cols);
    };
}
#endif