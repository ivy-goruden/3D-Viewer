#ifndef ALGEBRA
#define ALGEBRA

#include "../globals.h"

namespace s21{
class Algebra{
    public:
        Point3d static intersectLineRay3D(const Point3d& A, const Point3d& B,
            const Point3d& center, const Point3d& dir);
        void static getClosestPoint(std::optional<Point3d>& result, std::optional<Point3d>& intersect,
            const Point3d& center, float d, float far);
        std::optional<Point3d> static intersectSegmentPlane(const Point3d& A, const Point3d& B,
            const Point3d& n, float d);
        Vector_t getPolyNormal(std::vector<Point3d> &polygon, Point3d& center);
};
}

#endif