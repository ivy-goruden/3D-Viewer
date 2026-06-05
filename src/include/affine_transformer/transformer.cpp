#include "transformer.hpp"
#include "matrix.hpp"
#include <string>
#include <iostream>

namespace s21{

    Poly_Proj_t Transformer::getParallelProjection(Poly_t &polygons, const matrix_t &m, float scale){
        Poly_Proj_t projection;        
        for (auto poly : polygons){
            std::vector<Point> poly_projected = {};
            for (int dot : poly){
                float y = m[dot][1]*scale;
                float x = m[dot][0]*scale;
                Point newCoord = Point(x,y);
                poly_projected.push_back(newCoord);
            }
            projection.push_back(poly_projected);

        }
        return projection;
    }

    Poly_Proj_t Transformer::getPerspectiveProjection(Poly_t &polygons, const matrix_t &m, Camera& camera, 
            float scale) {
        Poly_Filled_t cropped_poly= cropFigure(polygons, m,camera);
        Poly_Proj_t projection;        
        for (auto poly : cropped_poly){
            std::vector<Point> poly_projected = {};
            for (Point3d dot : poly){
                float y = camera.d*dot.y/(dot.z - camera.z)*scale;
                float x = camera.d*dot.x/(dot.z - camera.z)*scale;
                Point newCoord = Point(x,y);
                poly_projected.push_back(newCoord);
            }
            projection.push_back(poly_projected);

        }
        return projection;
    }

    matrix_t Transformer::Rotate(int angleX_deg, int angleY_deg, int angleZ_deg, const matrix_t& original) {
        if (original.empty()){
            return matrix_t();
        }
        float angleX = angleX_deg * M_PI / 180.0;
        float angleY = angleY_deg * M_PI / 180.0;
        float angleZ = angleZ_deg * M_PI / 180.0;
        const matrix_t x_rotate = {
            {1,0,0,0},
            {0, std::cos(angleX), std::sin(angleX)*-1,0},
            {0, std::sin(angleX),std::cos(angleX), 0},
            {0,0,0,1}
        };
        const matrix_t y_rotate = {
            {std::cos(angleY), 0, std::sin(angleY), 0},
            {0,1,0,0},
            {std::sin(angleY)*-1, 0, std::cos(angleY), 0},
            {0,0,0,1}
        };
        const matrix_t z_rotate = {
            {std::cos(angleZ), std::sin(angleZ)*-1, 0, 0},
            {std::sin(angleZ),std::cos(angleZ),0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
        matrix_t result = Matrix::multiplyMatrix(&original, &x_rotate);
        result = Matrix::multiplyMatrix(&result, &y_rotate);
        result = Matrix::multiplyMatrix(&result, &z_rotate);
        return result;
    }

    matrix_t Transformer::Translate(float tx, float ty, float tz, const matrix_t& original) {
        if (original.empty()){
            return matrix_t();
        }
        const matrix_t translate = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {tx, ty, tz, 1}
        };
        return Matrix::multiplyMatrix(&original, &translate);
    }

    matrix_t Transformer::Scale(float sx, float sy, float sz, const matrix_t& original) {
        if (original.empty()){
            return matrix_t();
        }
        const matrix_t sc = {
            {sx, 0, 0, 0},
            {0, sy, 0, 0},
            {0, 0, sz, 0},
            {0, 0, 0, 1}
        };
        return Matrix::multiplyMatrix(&original, &sc);
    }

    bool Transformer::PolyVisible(std::vector<int> &polygon, const matrix_t &m, Camera& camera){
        float angle_rad = camera.getAngleRad();
        for (auto dot_num: polygon ){
            float z = m[dot_num][2]-camera.z;
            float x = m[dot_num][0];
            float y = m[dot_num][1];
            //хотя бы одна точка полигона не видна
            if ((x<-z*std::sin(angle_rad) || x>z*std::sin(angle_rad)) || (y<-z*std::sin(angle_rad) || y>z*std::sin(angle_rad)) || z<camera.d){
                return false;
            }
        }
        return true;
    }

    bool Transformer::PolyInvisible(std::vector<int> &polygon, const matrix_t &m, Camera& camera){
        float angle_rad = camera.getAngleRad();
        for (auto dot_num: polygon ){
            float z = m[dot_num][2] - camera.z;
            float x = m[dot_num][0];
            float y = m[dot_num][1];
            //хотя бы одна точка полигона видна
            if ((x>=-z*std::sin(angle_rad) && x<=z*std::sin(angle_rad)) && (y>=-z*std::sin(angle_rad) && y<=z*std::sin(angle_rad)) && z>camera.d){
                return false;
            }
        }
        return true;
    }

    std::vector<Point3d> Transformer::cropPoly(std::vector<int> &polygon, const matrix_t &m, Camera& camera){
        std::vector<Point3d> cropped_poly = {};
        int angle_deg = camera.getAngleDeg();
        float angle_rad = camera.getAngleRad();
        if (!polygon.empty()) {
            polygon.push_back(polygon[0]);
            for (size_t i = 0; i < polygon.size() - 1; ++i) {
                int dot_num = polygon[i];
                //координаты вершины
                float z = m[dot_num][2] - camera.z;
                float x = m[dot_num][0];
                float y = m[dot_num][1];
                //координаты второй вершины
                float z2 = m[polygon[i+1]][2] - camera.z;
                float x2 = m[polygon[i+1]][0];
                float y2 = m[polygon[i+1]][1];
                //координаты итоговой вершины
                float z_res = z;
                float x_res = x;
                float y_res = y;

                std::optional<Point3d> intersect = std::nullopt;
                std::optional<Point3d> result = std::nullopt;
                Point3d center = Point3d(camera.x, camera.y, camera.z);

                //лево
                if (x<-z*std::tan(angle_rad)){
                    
                    if (x2<=-z2*std::tan(angle_rad)){
                        continue; // ребро скрыто полностью
                    }
                    intersect = Algebra::intersectSegmentPlane(Point3d(x, y, z), Point3d(x2,y2,z2), Point3d(1, 0, tan(angle_rad)),0);
                    if (intersect != std::nullopt){
                        Algebra::getClosestPoint(result, intersect, center, camera.d, camera.far);
                    }


                }
                //право
                if(x>z*std::tan(angle_rad)){
                    if(x2>=z2*std::tan(angle_rad)){
                        continue;
                    }
                    intersect = Algebra::intersectSegmentPlane(Point3d(x, y, z), Point3d(x2,y2,z2), Point3d(-1, 0, tan(angle_rad)),0);
                    if (intersect != std::nullopt){
                        Algebra::getClosestPoint(result, intersect, center, camera.d, camera.far);
                    }
                }
                //верх
                if(y>z*std::tan(angle_rad)){
                    if((y2>=z2*std::tan(angle_rad))){
                        continue;
                    }
                    intersect = Algebra::intersectSegmentPlane(Point3d(x, y, z), Point3d(x2,y2,z2), Point3d(0, -1, tan(angle_rad/2)),0);
                    if (intersect != std::nullopt){
                        Algebra::getClosestPoint(result, intersect, center, camera.d, camera.far);
                    }
                }
                //низ
                if (y<-z*std::tan(angle_rad)){
                    if(y2<=-z2*std::tan(angle_rad)){
                        continue;
                    }
                    intersect = Algebra::intersectSegmentPlane(Point3d(x, y, z), Point3d(x2,y2,z2), Point3d(0, 1, tan(angle_rad)),0);
                    if (intersect != std::nullopt){
                        Algebra::getClosestPoint(result, intersect, center, camera.d, camera.far);
                    }
                }
                //перед
                if (z<camera.d){
                    if (z2<=camera.d){
                        continue;
                    }
                    intersect = Algebra::intersectSegmentPlane(Point3d(x, y, z), Point3d(x2,y2,z2), Point3d(0, 0, 1),-camera.d);
                    if (intersect != std::nullopt){
                        Algebra::getClosestPoint(result, intersect, center, camera.d, camera.far);
                    }
                }

                //конец
                if (z>camera.far){
                    if (z2>=camera.far){
                        continue;
                    }
                    intersect = Algebra::intersectSegmentPlane(Point3d(x, y, z), Point3d(x2,y2,z2), Point3d(0, 0, -1),camera.far);
                    if (intersect != std::nullopt){
                        Algebra::getClosestPoint(result, intersect, center, camera.d, camera.far);
                    }
                }

                if (!result.has_value()){
                    result = Point3d(x,y,z); // вершина видима
                }
                cropped_poly.push_back(*result);

            }
        }
        return cropped_poly;
    }

    Poly_Filled_t Transformer::cropFigure(Poly_t &polygons, const matrix_t &m, Camera& camera){
        Poly_Filled_t cropped = Poly_Filled_t();
        for (auto poly : polygons){
            // if (PolyInvisible(poly, m, camera)){
            //     continue; //игнорируем скрытые полигоны
            // }
            if (PolyVisible(poly, m, camera)){
                std::vector<Point3d> newPoly = {};
                for (int dot : poly){
                    newPoly.push_back(Point3d(m.at(dot)[0], m.at(dot)[1], m.at(dot)[2])); //если полигон виден полностью, добавляем без изменений
                }
                cropped.push_back(newPoly);
                continue;
            }
            cropped.push_back(cropPoly(poly, m, camera)); //обрезаем полигон, если он виден частично
        }
        return cropped;
    }

    matrix_t Transformer::perspective(matrix_t* original, const Camera *camera, float aspect) {
        if (original->empty()){
            return matrix_t();
        }
        float fov = camera->viewAngle_*2;
        float near = camera->d;
        float far = camera->far;
        float f = 1.0f / std::tan(fov * 3.1415926535f / 360.0f);
        const matrix_t sc = {
            {f / aspect, 0, 0, 0},
            {0, f, 0, 0},
            {0, 0, (far + near) / (near - far), (2.0f * far * near) / (near - far)},
            {0, 0, -1.0f, 0}
        };
        //Matrix::print_matrix(&sc, "naked perspective matrix");
        
        return Matrix::multiplyMatrix(&sc, &(*original));
    }   

    matrix_t Transformer::ortho(matrix_t* original, const Camera *camera, float aspect) {
        if (original->empty()){
            return matrix_t();
        }
        float fov = camera->viewAngle_*2;
        float tan = std::tan(fov * 3.1415926535f / 360.0f);
        float near = camera->d;
        float far = camera->far;
        float left = -1* (camera->z+camera->d)*tan;
        float bottom = left;
        float right = (camera->z+camera->d)*tan * aspect;
        float top = right;
        const matrix_t sc = {
            {2.0f / (right - left), 0, 0, -(right + left) / (right - left)},
            {0, 2.0f / (top - bottom), 0, -(top + bottom) / (top - bottom)},
            {0, 0, -2.0f / (far - near), -(far + near) / (far - near)},
            {0, 0, 0, 1.0f}
        };
        return Matrix::multiplyMatrix(&sc, &(*original));
    }

    inline void normalize(Vector_t &v){
        if (v.size() ==0){
            v = Vector_t(0,0,0);
            return;
        }
        v.x = v.x/v.size();
        v.y = v.y/v.size();
        v.z = v.z/v.size();
    }

    matrix_t Transformer::setView(matrix_t* original, 
            const Camera *camera, const Point3d center) {
        
        const Vector_t up = Vector_t(0,1,0);
        Point3d eye = Point3d(camera->x, camera->y, camera->z);

        // Вычисляем направление взгляда (forward)
        Vector_t f = Vector_t(center, eye);
        normalize(f);  // нормируем

        // Вычисляем правый вектор (right) = forward × up
        Vector_t r = up^f;
        normalize(r);

        // Вычисляем настоящий up = right × forward
        Vector_t u = f^r;

        Vector_t eye_v = Vector_t(eye.x,eye.y, eye.z);
        
        // const matrix_t sc = {
        //     {r.x, u.x, -f.x, 0},
        //     {r.y, u.y, -f.y, 0},
        //     {r.z,  u.z, -f.z, 0},
        //     {r*eye_v *-1,  u*eye_v*-1, f*eye_v, 1.0f}
        // };
        const matrix_t sc = {
            { r.x,   r.y,   r.z,   r*eye_v },
            { u.x,  u.y,  u.z,  u*eye_v*-1 },
            { f.x*-1, f.y*-1, f.z, f*eye_v*-1 },
            { 0, 0, 0, 1 }
        };
        //Matrix::print_matrix(&sc, "naked view matrix");
        return Matrix::multiplyMatrix(&sc, &(*original));
    }



}
