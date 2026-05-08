#ifndef PROJECTION_H
#define PROJECTION_H

#include "../globals.h"

namespace s21 {

    class Projection {
      protected:
        double distance;
        double view_w;
        double view_h;
        int screen_w;
        int screen_h;
      public:
        Projection(double d, double vw, double vh, int sw, int sh) : distance(d), view_w(vw), view_h(vh), screen_w(sw), screen_h(sh) {};
        virtual ~Projection() = default;
        virtual Vert_t calculate(matrix_t& matrix) = 0;
    };

    class OrtoProjection : public Projection {
      private:
        const matrix_t matrix = {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
      public:
        OrtoProjection(double d, double vw, double vh, int sw, int sh) : Projection(d, vw, vh, sw, sh) {};
        ~OrtoProjection() override = default;
        Vert_t calculate(matrix_t& matrix) override;
    };
    
    class PerspProjection : public Projection {
      private:
      public:
        PerspProjection(double d, double vw, double vh, int sw, int sh) : Projection(d, vw, vh, sw, sh) {};
        ~PerspProjection() override = default;
        Vert_t calculate(matrix_t& matrix) override;
    };
    
}

#endif
