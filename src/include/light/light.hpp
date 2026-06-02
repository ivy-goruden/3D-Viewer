#ifndef LIGHT
#define LIGHT

#include "../globals.h"

namespace s21{
class Dot_Light{
    public:
        Dot_Light(Rgb, Point3d, float);
        Rgb color_;
        Point3d position_;
        float intensity_;
};


typedef std::vector<Dot_Light*> Light_t;

class Light{
    public:
        Light(float);
        Light_t lights_;
        float ambient_;
        //Rgb getColor(Rgb &poly_color, Point3d& dot) const;
        ~Light();
};
}
#endif