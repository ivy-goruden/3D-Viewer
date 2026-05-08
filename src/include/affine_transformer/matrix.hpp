#include "../globals.h"

namespace s21 {

    class Matrix {
      public:
        Matrix() {};
        ~Matrix() {};

        static matrix_t multiplyMatrix(const matrix_t *f, const matrix_t *s){
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

        static matrix_t createMatrix(int rows, int cols){
            std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0));
            return matrix;
        }

    };

}
