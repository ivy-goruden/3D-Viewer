#include "figure.hpp"

namespace s21{

    Vert_t Figure::getVertices(){
        return vertices_;
    }
    Poly_t Figure::getPolygons(){
        return polygons_;
    }
    matrix_t Figure::getMatrix(){
        return matrix_;
    }
    void static Figure::genMatrix(){
        //генерация матрицы 4(x,y,z,1) на N(кол-во вершин)
    }
    int Figure::getVerticesNum(){
        return vertices_.size();
    }
    int Figure::getNodesNum(){
        return nodes_.size();
    }
}