#ifndef GLOBALS
#define GLOBALS

#include <cmath>
#include <cstdio>
#include <format>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <map>

#define MOVE_SCOPE 0.1
#define SCALING_FACTOR 0.1
#define ROTATE_SCOPE 10

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace s21 {

struct Point {
    float x, y;
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return ((x == other.x && y == other.y)); }
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct Point3d {
    float x, y, z;
    Point3d(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Point3d operator-(const Point3d& other) const { return Point3d(x - other.x, y - other.y, z - other.z); }
};

struct Transform {
    float scale;
    int rotation_x, rotation_y, rotation_z;
    float trans_x, trans_y, trans_z;
    std::string toString() {
        return std::format("sc {} rx {} ry {} rz {} tx {} ty {} tz {}", scale, rotation_x, rotation_y, rotation_z,
                           trans_x, trans_y, trans_z);
    }
};

struct Seg_t {
    int start, end;
    bool operator==(const Seg_t& other) const {
        return ((start == other.start && end == other.end) || (start == other.end && end == other.start));
    }
    bool operator<(const Seg_t& other) const {
        int a1 = std::min(start, end), b1 = std::max(start, end);
        int a2 = std::min(other.start, other.end), b2 = std::max(other.start, other.end);
        if (a1 != a2) return a1 < a2;
        return b1 < b2;
    }
};

struct Vector_t {
    float x, y, z;
    Vector_t(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector_t(Point3d& start, Point3d& end) : x(end.x - start.x), y(end.y - start.y), z(end.z - start.z) {}

    Vector_t operator-(const Point3d& P) const { return Vector_t(x - P.x, y - P.y, z - P.z); }
    Vector_t operator+(const Point3d& P) const { return Vector_t(x + P.x, y + P.y, z + P.z); }
    Vector_t operator*(const float N) const { return Vector_t(x * N, y * N, z * N); }
    // скалярное произведение
    float operator*(const Vector_t& s) const { return x * s.x + y * s.y + z * s.z; }
    // векторное
    Vector_t operator^(const Vector_t& s) const {
        float Rx = y * s.z - z * s.y;
        float Ry = z * s.x - x * s.z;
        float Rz = x * s.y - y * s.x;
        return Vector_t(Rx, Ry, Rz);
    }
    size_t size() const { return std::sqrt(x * x + y * y + z * z); }
};

struct Line_t {
    Point start, end;
    Line_t(Point s, Point e) : start(s), end(e) {}
    bool operator==(const Line_t& other) const {
        return ((start == other.start && end == other.end) || (start == other.end && end == other.start));
    }
    bool operator<(const Line_t& other) const {
        // Приводим текущую линию к каноническому виду ProjectedVertex(упорядоченные концы)
        auto canonical = [](const Point& p1, const Point& p2) -> std::pair<Point, Point> {
            if (p1 < p2)
                return {p1, p2};
            else
                return {p2, p1};
        };
        auto [this_start, this_end] = canonical(start, end);
        auto [other_start, other_end] = canonical(other.start, other.end);

        if (this_start == other_start) return this_end < other_end;
        return this_start < other_start;
    }
};

typedef std::vector<std::vector<float>> matrix_t;        // matrix for affine transformations
typedef std::vector<Point> Vert_t;                        // vertices вершины
typedef std::vector<std::vector<int>> Poly_t;             // polygons
typedef std::vector<std::vector<Point3d>> Poly_Filled_t;  // polygons with vertices filled
typedef std::vector<std::vector<Point>> Poly_Proj_t;      // projection of polygons
typedef std::vector<Seg_t> Edge_t;                        // edges грани

// typedef std::map<Point3d, Rgb> Colour_Map_t;  // цвета пикселей фигуры
// typedef std::map<Point, Rgb> Color_Mask_t;    // цвета пикселей проекции фигуры

struct ProjectedVertex {
    int x, y;        // экранные координаты (пиксели)
    float depth;    // глубина (z-буфер)
    ProjectedVertex(int x = 0, int y = 0, float depth = 0) : x(x), y(y), depth(depth) {}
};

enum class Action { QUIT_ACT, ROTATE_ACT, MOVE_ACT, LOAD_FIGURE_ACT, SCALE_ACT };

struct Bounds {
    float minx, maxx;
    float miny, maxy;
    float minz, maxz;
    std::string toString() { return std::format("x{}:{} y{}:{} z{}:{}", minx, maxx, miny, maxy, minz, maxz); }
};

struct Rgb {
    float red;
    float green;
    float blue;
    float alpha;
    Rgb() : red(0), green(0), blue(0), alpha(0) {}

    Rgb(float r, float g, float b, float a = 1) : red(r), green(g), blue(b), alpha(a) {}

    Rgb operator*(Rgb& other) const { return Rgb(red * other.red, green * other.green, blue * other.blue); }

    Rgb operator*(float other) const { return Rgb(red * other, green * other, blue * other); }
};

}  // namespace s21

#endif