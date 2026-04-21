#ifndef TRANSFORMER
#define TRANSFORMER
#include "../globals.h"
namespace s21{
    
    //Класс афинных преобразований фигур
    class Transformer{
        public:
            Transformer();
            matrix_t static Rotate(int angleX, int angleY, int angleZ, const matrix_t&);
            Vert_t static getParallelProjection(matrix_t);
            Vert_t static getPerspectiveProjection(matrix_t m);
            matrix_t static multiplyMatrix(const matrix_t *f, const matrix_t *s);
            matrix_t static createMatrix(int rows, int cols);
    };
}
#endif