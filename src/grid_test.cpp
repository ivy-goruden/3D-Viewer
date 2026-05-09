#include "./include/globals.h"
#include "./include/grid.hpp"
#include "./include/projection/projection.hpp"
#include <iostream>
#include <cmath>

int main() {
    s21::Grid grid = s21::Grid();
    grid.createGrid(0, 1920, 0, 780, 0, 2000, 100);
    s21::matrix_t m = grid.getMatrix();

    // for (auto &&i : m) {
    //     for (auto &&j : i) {
    //         std::cout << j << " ";
    //     }
    //     std::cout << std::endl;
    // }
    
    s21::PerspProjection proj(-1000, 1920, 780, 1920, 780);
    s21::Vert_t points = proj.calculate(m);
    for (auto &&i : points) {
        std::cout << round(i.x)  << " " << i.y << "; ";
    }

    return 0;
}
