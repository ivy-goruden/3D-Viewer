#include "figure.hpp"
namespace s21{
    Figure::Figure(matrix_t m, Poly_t p, Node_t n): matrix_(m), polygons_(p), nodes_(n){
        projectionVertices_ = Vert_t();
    }

    Figure::Figure(ObjLoader loader){
        polygons_ = Poly_t();
        projectionVertices_ = Vert_t();

        matrix_t m = matrix_t();
        for (VertexObj_t v : loader.vertices) {
            m.push_back({v.x, v.y, v.z, 1.0});
        }
        matrix_ = std::move(m);

        Node_t l = Node_t();
        // for (const auto& line : loader.lines) {
        //     l.push_back(Point(line.vi, line.ti));
        // }
        nodes_ = std::move(l);
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
    Node_t Figure::getNodes(){
        return nodes_;
    }

    int Figure::getNodesNum(){
        //return nodes_.size();
        return 0;
    }
}