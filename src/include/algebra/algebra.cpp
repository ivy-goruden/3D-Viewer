#include <optional>
#include "algebra.hpp"
namespace s21{
    Vector_t Algebra::getPolyNormal(std::vector<Point3d> &poly, Point3d& center){
        if (poly.size() <3){ return Vector_t(0,0,0);}
        Vector_t V = Vector_t(poly[0], poly[1]);
        Vector_t W = Vector_t(poly[0], poly[2]);
        Vector_t R = W^V;
        if ((R+poly[0] - center).size() < (R*-1 + poly[0] -center).size()){
            R = R*-1;
        }
        return R;
    }

    std::optional<Point3d> Algebra::intersectSegmentPlane(const Point3d& A, const Point3d& B,
                                             const Point3d& n, float d) {
        Point3d dir = B - A;
        float denom = n.x * dir.x + n.y * dir.y + n.z * dir.z;
        if (std::fabs(denom) < 1e-9) return std::nullopt; // параллельны

        float t = -(n.x * A.x + n.y * A.y + n.z * A.z + d) / denom;
        if (t < 0.0 || t > 1.0) return std::nullopt;

        return Point3d(A.x + t * dir.x, A.y + t * dir.y, A.z + t * dir.z);
    }

    void Algebra::getClosestPoint(std::optional<Point3d>& result, std::optional<Point3d>& intersect,
                     const Point3d& center, float d, float far) {
        if (!intersect.has_value()){
            return;
        }
        float dist_intersect = std::hypot(intersect->x - center.x,
                                        intersect->y - center.y,
                                        intersect->z - center.z);
        if (!result.has_value()) {
            result = intersect;
        } else {
            float dist_result = std::hypot(result->x - center.x,
                                            result->y - center.y,
                                            result->z - center.z);
            if (dist_intersect < dist_result) {
                result = intersect;
            }
        }
    }

    Point3d Algebra::intersectLineRay3D(const Point3d& A, const Point3d& B,
                           const Point3d& center, const Point3d& dir) {
        float dx = B.x - A.x;
        float dy = B.y - A.y;
        float dz = B.z - A.z;
        
        float rx = center.x - A.x;
        float ry = center.y - A.y;
        float rz = center.z - A.z;
        
        // Выбираем две координаты с максимальными компонентами направления прямой,
        // чтобы определитель был максимальным (избегаем деления на ноль)
        int i1 = 0, i2 = 1; // индексы для x,y по умолчанию
        float abs_dx = std::fabs(dx), abs_dy = std::fabs(dy), abs_dz = std::fabs(dz);
        
        if (abs_dx >= abs_dy && abs_dx >= abs_dz) {
            // Используем плоскости yz
            i1 = 1; i2 = 2;
        } else if (abs_dy >= abs_dx && abs_dy >= abs_dz) {
            // Используем плоскости xz
            i1 = 0; i2 = 2;
        }
        // Иначе оставляем xy (i1=0, i2=1)
        
        // Получаем компоненты в выбранных осях
        float AB1 = (i1 == 0 ? dx : (i1 == 1 ? dy : dz));
        float AB2 = (i2 == 0 ? dx : (i2 == 1 ? dy : dz));
        float dir1 = (i1 == 0 ? dir.x : (i1 == 1 ? dir.y : dir.z));
        float dir2 = (i2 == 0 ? dir.x : (i2 == 1 ? dir.y : dir.z));
        float r1 = (i1 == 0 ? rx : (i1 == 1 ? ry : rz));
        float r2 = (i2 == 0 ? rx : (i2 == 1 ? ry : rz));
        
        // Определитель системы (для t и u)
        float det = dir1 * AB2 - AB1 * dir2;
        // По условию det != 0 (прямые не параллельны)
        
        float t = (dir1 * r2 - r1 * dir2) / det;
        // float u = (AB1 * r2 - r1 * AB2) / det; // не требуется
        
        // Точка пересечения
        Point3d P(A.x + t * dx, A.y + t * dy, A.z + t * dz);
        return P;
    }
}