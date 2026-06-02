#include "camera.hpp"

using namespace s21;
Camera::Camera(float x, float y, float z, int viewAngle, float d, float far) : x(x), y(y), z(z),viewAngle_(viewAngle), d(d), far(far){}

float Camera::angleToRad(int angle){
    return angle * M_PI / 180.0;
}

float Camera::getAngleRad(){
    return angleToRad(viewAngle_);
}
int Camera::getAngleDeg(){
    return viewAngle_;
}