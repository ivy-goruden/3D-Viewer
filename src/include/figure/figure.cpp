#include "figure.hpp"
namespace s21{
    Figure::Figure(matrix_t m, Poly_t p, Edge_t n): matrix_(m), polygons_(p), edges_(n){
        projectionVertices_ = Vert_t();
        minz = 0;
    }
    Figure::Figure(ObjLoader loader){
        projectionVertices_ = Vert_t();

        matrix_t m = matrix_t();
        for (VertexObj_t v : loader.vertices) {
            m.push_back({v.x, v.y, v.z, 1.0});
        }
        matrix_ = std::move(m);
        
        minz = 0;
        for (int i = 0; i < matrix_.size(); i++) {
            if (matrix_[i][2] < 0 && minz > matrix_[i][2]) minz = matrix_[i][2];
        }

        Edge_t l = Edge_t();
        for (const auto& line : loader.lines) {
            if (line.size() <2) {continue;}
            for (auto i = 0; i < line.size()-1; i++){
                int start = line[i].vi-1;
                int end = line[i+1].vi-1;
                printf("strt: %d, end: %d", start, end);
                if (start >=0 && start < matrix_.size() && end >=0 && end <matrix_.size()){
                    Seg_t segment = Seg_t(start, end);
                    l.push_back(segment);
                }
            }
        }
        Unique(l);
        edges_ = std::move(l);
        polygons_ = Poly_t();

        // без учета групп
        for (FaceObj_t face: loader.faces){
            std::vector<int> poly;
            for (FaceElementObj_t el: face){
                poly.push_back(el.vi-1);
            }
            polygons_.push_back(poly);
        }
        
        // анализ групп
        // if (loader.groups.size() > 0) {
        //     printf("group size %ld\n", loader.groups.at(0).size());
        //     for (s21::GroupElement_t e : loader.groups.at(0)) {
        //         if (e.kind == FACE) {
        //             std::vector<int> poly;
        //             for (s21::FaceElementObj_t el : loader.faces.at(e.index)){
        //                 poly.push_back(el.vi-1);
        //             }
        //             polygons_.push_back(poly);
        //         }
        //     }
        // } else {
        //     for (s21::FaceObj_t f : loader.faces) {
        //         std::vector<int> poly;
        //         for (FaceElementObj_t el: f) {
        //             poly.push_back(el.vi-1);
        //         }
        //         polygons_.push_back(poly);
        //     }
        // }
    }

    void Figure::Unique(Edge_t &vec){
        std::sort(vec.begin(), vec.end());
        auto last = std::unique(vec.begin(), vec.end());
        vec.erase(last, vec.end());
    }

    Vert_t Figure::getProjection(){
        return projectionVertices_;
    }

    double Figure::getMinz() {
        return minz;
    }

    double Figure::getMinz(matrix_t& m) {
        double mz = 0;
        for (int i = 0; i < m.size(); i++) {
            if (m[i][2] < 0 && mz > m[i][2]) mz = m[i][2];
        }
        return mz;
    }

    Poly_t Figure::getPolygons()
    {
        return polygons_;
    }
    matrix_t Figure::getMatrix(){
        return matrix_;
    }
    int Figure::getVerticesNum(){
        return matrix_.size();
    }
    Edge_t Figure::getEdges(){
        return edges_;
    }

    int Figure::getNodesNum(){
        return edges_.size();
    }
}