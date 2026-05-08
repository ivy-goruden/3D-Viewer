#ifndef PROJECTION_H
#define PROJECTION_H

#include "../globals.h"

namespace s21 {

    class Projection {
      private:
        /* data */
      public:
        Projection() {};
        ~Projection() {};
        virtual Vert_t calculate() = 0;
    };

    class OrtoProjection : Projection {
      private:
        const matrix_t matrix = {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
      public:
        OrtoProjection();
        ~OrtoProjection();
        Vert_t calculate();
    };
    
}

#endif
