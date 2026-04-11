#ifndef GLOBALS
#define GLOBALS

#define MOVE_SCOPE 0.1
#define SCALE 0.1
#define ROTATE_SCOPE 10

#include "obj_parser/obj_parser.hpp"
#include "figure/figure.hpp"
#include "affine_transformer/transformer.hpp"

namespace s21{
    typedef std::list<std::list<double>> matrix_t;               //матрица представления фигуры
    #typedef std::array<std::pair<double, double>> Vert_t;       //vertices
    #typedef std::list<std::list<Vert_t::iterator>> Poly_t;      //polygons
    #typedef std::list<std::pair<double, double>> Node_t;        //nodes
}

typedef enum {
    QUIT,
    ROTATE,
    MOVE,
    LOAD_FIGURE,
    SCALE
}

//ANGLES
//angles are counted from Y up-looking axis

//angles of axis in 2D system
#define X2 90
#define Y2 0
#define Z2 90

//default axis angle
#define X2 135
#define Y2 90
#define Z2 0

#endif