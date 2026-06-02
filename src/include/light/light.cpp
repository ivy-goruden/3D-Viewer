#include "light.hpp"

namespace s21{

    // Rgb Light::getColor(Rgb &poly_color, Point3d& dot) const{
    //     //Vector_t N = Algebra::getPolyNormal(polygon);
    //     float i = ambient_;
    //     Rgb color = poly_color;
    //     for (auto light : lights_){
    //         Vector_t lVector = Vector_t(light->position_ ,dot);
    //         i= N*lVector * light->intensity_ / N.size() * lVector.size();
    //         color = color*light->color_*i;
    //     }
    //     return color;
    // };

    Bounds static getPolyBounds(std::vector<Point3d> &polygon){
        if (polygon.empty()) {
            return Bounds(0, 0, 0, 0, 0, 0);
        }
        int maxx = polygon[0].x;
        int maxy = polygon[0].y;
        int minx = polygon[0].x;
        int miny = polygon[0].y;
        int minz = polygon[0].z;
        int maxz = polygon[0].z;
        for (const auto& dot : polygon) {
            if (dot.x < minx) minx = dot.x;
            if (dot.x > maxx) maxx = dot.x;
            if (dot.y < miny) miny = dot.y;
            if (dot.y > maxy) maxy = dot.y;
            if (dot.z < minz) minz = dot.z;
            if (dot.z > maxz) maxz = dot.z;
        }
        return Bounds(minx, maxx, miny, maxy, minz, maxz);
    }

    Light::Light(float ambient): ambient_(ambient){}
    Light::~Light(){
        for (auto light: lights_){
            delete light;
        }
    };

    Dot_Light::Dot_Light(Rgb color, Point3d pos, float intensity) : color_(color), position_(pos), intensity_(intensity){}

}