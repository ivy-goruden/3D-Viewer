#ifndef TRANSFORMER
#define TRANSFORMER

#include "../globals.h"

namespace s21{
    
    //Класс афинных преобразований фигур
    class Transformer{
        public:
            Transformer() {};
            matrix_t static Translate(double tx, double ty, double tz, const matrix_t&);
            matrix_t static Rotate(int angleX, int angleY, int angleZ, const matrix_t&);
            matrix_t static Scale(double sx, double sy, double sz, const matrix_t&);
            Vert_t static getParallelProjection(matrix_t, double);
            Vert_t static getPerspectiveProjection(matrix_t m, double, double, double);
    };
}

#endif
