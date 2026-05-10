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

    class SimplePerspective {
      private:
        double distance;
        int screenW;
        int screenH;
        
      public:
        SimplePerspective(double d, int w, int h) 
            : distance(d), screenW(w), screenH(h) {}
        
        Point project(const Point3d& point) {
            if (point.z <= 0) return {-1, -1};            
            double scale = distance / point.z;
            int x = (int)(point.x * scale + screenW / 2);
            int y = (int)(screenH / 2 - point.y * scale);            
            return {x, y};
        }

        std::pair<Point, Point> projectSegment(const Point3d& p1, const Point3d& p2) {
            return {project(p1), project(p2)};
        }    
    };

    // class PerspectiveProjection {
    //   private:
    //     // Параметры камеры
    //     Point3d eye;      // положение камеры
    //     Point3d center;   // куда смотрим
    //     Point3d up;       // вектор вверх
        
    //     // Параметры проекции
    //     double fov;       // угол обзора в градусах
    //     double aspect;    // ширина/высота экрана
    //     double nearPlane; // ближняя плоскость
    //     double farPlane;  // дальняя плоскость
        
    //     int screenWidth;
    //     int screenHeight;
        
    //     // Матрица вида (View Matrix)
    //     double viewMatrix[4][4];
        
    //     // Матрица перспективной проекции (Projection Matrix)
    //     double projMatrix[4][4];
        
    //   public:
    //     PerspectiveProjection(Point3d cameraPos, Point3d lookAt, Point3d upVector,
    //                         double fovDeg, double aspectRatio, 
    //                         double nearP, double farP,
    //                         int sw, int sh) {
    //         eye = cameraPos;
    //         center = lookAt;
    //         up = upVector;
    //         fov = fovDeg;
    //         aspect = aspectRatio;
    //         nearPlane = nearP;
    //         farPlane = farP;
    //         screenWidth = sw;
    //         screenHeight = sh;
            
    //         calculateViewMatrix();
    //         calculateProjectionMatrix();
    //     }
        
    //     // Проекция точки 3D -> 2D (экранные координаты)
    //     Point project(const Point3d& point) {
    //         // 1. Переводим точку в пространство камеры
    //         double cameraSpace[4] = {point.x, point.y, point.z, 1.0};
    //         multiplyMatrixVector(viewMatrix, cameraSpace);
            
    //         // 2. Применяем перспективную проекцию
    //         double clipSpace[4];
    //         multiplyMatrixVector(projMatrix, cameraSpace, clipSpace);
            
    //         // 3. Перспективное деление (преобразование в NDC - Normalized Device Coordinates)
    //         if (clipSpace[3] == 0) return {-1, -1};
            
    //         double ndcX = clipSpace[0] / clipSpace[3];
    //         double ndcY = clipSpace[1] / clipSpace[3];
            
    //         // 4. Преобразование в экранные координаты
    //         int screenX = (int)((ndcX + 1.0) * 0.5 * screenWidth);
    //         int screenY = (int)((1.0 - (ndcY + 1.0) * 0.5) * screenHeight); // Y перевернут
            
    //         return {screenX, screenY};
    //     }
        
    //     // Проекция отрезка
    //     std::pair<Point, Point> projectSegment(const Point3d& p1, const Point3d& p2) {
    //         return {project(p1), project(p2)};
    //     }
        
    // private:
    //     void calculateViewMatrix() {
    //         // Вычисляем векторы камеры
    //         Point3d forward = normalize({
    //             center.x - eye.x,
    //             center.y - eye.y,
    //             center.z - eye.z
    //         });
            
    //         Point3d right = normalize(cross(forward, up));
    //         Point3d trueUp = normalize(cross(right, forward));
            
    //         // Матрица вида (LookAt матрица)
    //         viewMatrix[0][0] = right.x; viewMatrix[0][1] = right.y; viewMatrix[0][2] = right.z; viewMatrix[0][3] = -dot(right, eye);
    //         viewMatrix[1][0] = trueUp.x; viewMatrix[1][1] = trueUp.y; viewMatrix[1][2] = trueUp.z; viewMatrix[1][3] = -dot(trueUp, eye);
    //         viewMatrix[2][0] = -forward.x; viewMatrix[2][1] = -forward.y; viewMatrix[2][2] = -forward.z; viewMatrix[2][3] = dot(forward, eye);
    //         viewMatrix[3][0] = 0; viewMatrix[3][1] = 0; viewMatrix[3][2] = 0; viewMatrix[3][3] = 1;
    //     }
        
    //     void calculateProjectionMatrix() {
    //         double f = 1.0 / tan(fov * 0.5 * M_PI / 180.0);
    //         double rangeInv = 1.0 / (nearPlane - farPlane);
            
    //         projMatrix[0][0] = f / aspect; projMatrix[0][1] = 0; projMatrix[0][2] = 0; projMatrix[0][3] = 0;
    //         projMatrix[1][0] = 0; projMatrix[1][1] = f; projMatrix[1][2] = 0; projMatrix[1][3] = 0;
    //         projMatrix[2][0] = 0; projMatrix[2][1] = 0; projMatrix[2][2] = (farPlane + nearPlane) * rangeInv; projMatrix[2][3] = 2 * farPlane * nearPlane * rangeInv;
    //         projMatrix[3][0] = 0; projMatrix[3][1] = 0; projMatrix[3][2] = -1; projMatrix[3][3] = 0;
    //     }
        
    //     void multiplyMatrixVector(const double matrix[4][4], const double vec[4], double result[4]) {
    //         for (int i = 0; i < 4; i++) {
    //             result[i] = matrix[i][0] * vec[0] + matrix[i][1] * vec[1] + 
    //                         matrix[i][2] * vec[2] + matrix[i][3] * vec[3];
    //         }
    //     }
    // };

}

#endif
