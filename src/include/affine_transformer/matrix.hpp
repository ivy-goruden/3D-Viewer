#include "../globals.h"
#include <format>
#include <string>
#include <iostream>

namespace s21 {

    class Matrix {
      public:
        Matrix() {};
        ~Matrix() {};

        static matrix_t multiplyMatrix(const matrix_t *f, const matrix_t *s) {
            int rows = f->size();
            int cols = (*s)[0].size();
            int inner = s->size();
            matrix_t nMatrix = createMatrix(rows, cols);
            for (int row = 0; row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    float val = 0;
                    for (int x = 0; x < inner; x++) {
                        val += ((*f)[row][x])*((*s)[x][col]);
                    }
                    nMatrix[row][col] = val;
                }
            }
            return nMatrix;
        }

        static matrix_t createMatrix(int rows, int cols) {
            std::vector<std::vector<float>> matrix(rows, std::vector<float>(cols, 0));
            return matrix;
        }

        static Bounds getBounds(matrix_t& matrix_) {
            if (matrix_.empty()) {
                return {0,0,0,0,0,0};
            }
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
    };

}
