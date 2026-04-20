#include "figure.hpp"
namespace s21{
    Figure::Figure(matrix_t m, Poly_t p): matrix_(m), polygons_(p){}

    Figure::Figure(ObjLoader loader){
        polygons_ = Poly_t();

        matrix_t m = matrix_t();
        for (VertexObj_t v : loader.vertices) {
            m.push_back({v.x, v.y, v.z, 1.0});
        }
        matrix_ = std::move(m);
    }

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