#include "axes.hpp"

#include "globals.h"

namespace s21 {
Axes::Axes() {
    m = matrix_t();
    m.push_back({0, 0, 0, 1});
    m.push_back({20, 0, 0, 1});
    m.push_back({0, 0, 0, 1});
    m.push_back({0, 20, 0, 1});
    m.push_back({0, 0, 0, 1});
    m.push_back({0, 0, 20, 1});
}

Axes::~Axes() {
    //
}

matrix_t Axes::getMatrix() { return m; }

}  // namespace s21
