#include "./include/grid.hpp"
#include <iostream>

int main() {
    s21::Grid grid = s21::Grid();
    grid.createGrid(0, 1, 0, 1, 0, 1, 0.1);

    for (auto &&i : grid.getMatrix()) {
        for (auto &&j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    

    return 0;
}
