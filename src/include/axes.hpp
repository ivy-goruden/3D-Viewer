#ifndef AXES_HPP
#define AXES_HPP

#include "globals.h"

namespace s21 {

    class Axes {
    private:
        matrix_t m;

    public:
        Axes();
        ~Axes();
        matrix_t getMatrix();
    };

}

#endif
