#include "figure.hpp"
namespace s21{
    Figure::Figure(matrix_t m, Poly_t p): matrix_(m), polygons_(p){}

    Vert_t Figure::getProjection(){
        return projectionVertices_;
    }
    Poly_t Figure::getPolygons(){
        return polygons_;
    }
    matrix_t Figure::getMatrix(){
        return matrix_;
    }
    int Figure::getVerticesNum(){
        return matrix_.size();
    }
    int Figure::getNodesNum(){
        //return nodes_.size();
        return 0;
    }
}