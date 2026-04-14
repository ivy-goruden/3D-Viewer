#ifndef GLOBALS
#define GLOBALS

#include <utility>
#include <vector>
#include <list>
#include <memory>

#define MOVE_SCOPE 0.1
#define SCALING_FACTOR 0.1
#define ROTATE_SCOPE 10
namespace s21{

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

typedef std::vector<std::vector<double>> matrix_t;      //matrix for affine transformations
typedef std::vector<Point> Vert_t;                      //vertices
typedef std::vector<std::vector<int>> Poly_t;           //polygons
//typedef std::list<Point> Node_t;                      //nodes 

enum class Action {
    QUIT_ACT,
    ROTATE_ACT,
    MOVE_ACT,
    LOAD_FIGURE_ACT,
    SCALE_ACT
};

typedef struct {
    std::unique_ptr<matrix_t> matrix;
    std::unique_ptr<Poly_t> polygons;
} FigureData_t;

typedef struct{
    Vert_t *vertices;
    Poly_t *polygons;

} ProjectionData_t; // figure data for drawing

//ANGLES
//angles are counted from Y up-looking axis

//angles of axis in 2D system
#define X2 90
#define Y2 0
#define Z2 90

//default axis angle
#define X2_d 135
#define Y2_d 90
#define Z2_d 0
}
#endif