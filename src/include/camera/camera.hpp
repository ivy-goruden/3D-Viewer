#ifndef CAMERA
#define CAMERA

#include "../globals.h"
namespace s21{
class Camera {
    public:
        float x,y,z;
        int viewAngle_;
        float d; //расстояние до экрана
        float far; //расстояние до дальней грани отсечения

        Camera(float,float,float,int,float, float);
        float angleToRad(int);
        float getAngleRad();
        int getAngleDeg();
};
}
#endif