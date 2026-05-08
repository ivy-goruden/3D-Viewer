#include "projection.hpp"
#include <cmath>

s21::Vert_t s21::OrtoProjection::calculate(s21::matrix_t& matrix) {
    s21::Vert_t points;
    for (int i = 0; i < matrix.size(); i++) {
        int x = floor(matrix[i][0] * view_w / screen_w);
        int y = floor(matrix[i][1] * view_h / screen_h);
        points.push_back({x, y});
    }
    return points;
}

s21::Vert_t s21::PerspProjection::calculate(s21::matrix_t& matrix) {
    s21::Vert_t points;
    for (int i = 0; i < matrix.size(); i++) {
        double z = (matrix[i][2] == 0) ? 0.0001 : matrix[i][2];
        double ppx = distance * matrix[i][0] / z;
        double ppy = distance * matrix[i][1] / z;
        int x = static_cast<int>(floor(ppx * screen_w / view_w));
        int y = static_cast<int>(floor(ppy * screen_h / view_h));
        points.push_back({x, y});
    }
    return points;
}
