#ifndef GLOBALS
#define GLOBALS

#include <cstdio>
#include <format>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#define MOVE_SCOPE 0.1
#define SCALING_FACTOR 0.1
#define ROTATE_SCOPE 10

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace s21 {

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

struct Point3d {
    double x, y, z;
    Point3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

struct Transform {
    double scale;
    int rotation_x, rotation_y, rotation_z;
    double trans_x, trans_y, trans_z;
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

typedef std::vector<std::vector<double>> matrix_t;  // matrix for affine transformations
typedef std::vector<Point> Vert_t;                  // vertices вершины
typedef std::vector<std::vector<int>> Poly_t;       // polygons
typedef std::vector<Seg_t> Edge_t;                  // edges грани

enum class Action { QUIT_ACT, ROTATE_ACT, MOVE_ACT, LOAD_FIGURE_ACT, SCALE_ACT };

typedef struct {
    std::unique_ptr<matrix_t> matrix;
    std::unique_ptr<Poly_t> polygons;
} FigureData_t;

typedef struct {
    Vert_t* vertices;
    Poly_t* polygons;

} ProjectionData_t;  // figure data for drawing

struct Bounds {
    double minx, maxx;
    double miny, maxy;
    double minz, maxz;
    std::string toString() { return std::format("x{}:{} y{}:{} z{}:{}", minx, maxx, miny, maxy, minz, maxz); }
};

// ANGLES
// angles are counted from Y up-looking axis

// angles of axis in 2D system
#define X2 90
#define Y2 0
#define Z2 90

// default axis angle
#define X2_d 135
#define Y2_d 90
#define Z2_d 0

}  // namespace s21

#endif