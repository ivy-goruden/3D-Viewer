#include "figure.hpp"
#include "../affine_transformer/matrix.hpp"

namespace s21{

    // Figure::Figure(matrix_t& m, FaceObj_t p, Edge_t n): matrix_(m), polygons_(p), edges_(n){
    //     projectionVertices_ = Vert_t();
    //     minz = 0.0;
    //     bounds = {0, 0, 0, 0, 0, 0};
    //     projectionVertices_ = Vert_t();
    // }

    Figure::Figure(ObjLoader loader){
        minz = 0.0;
        bounds = {0, 0, 0, 0, 0, 0};
        projectionVertices_ = Vert_t();

        matrix_t m = matrix_t();
        for (VertexObj_t v : loader.vertices) {
            m.push_back({v.x, v.y, v.z, 1.0});
        }
        matrix_ = std::move(m);
        bounds = Matrix::getBounds(matrix_);
        
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

        polygons_ = loader.faces;
        textures_ = loader.textures;
        
    }

    void Figure::Unique(Edge_t &vec) const {
        std::sort(vec.begin(), vec.end());
        auto last = std::unique(vec.begin(), vec.end());
        vec.erase(last, vec.end());
    }

    Vert_t Figure::getProjection(){
        return projectionVertices_;
    }

    std::vector<FaceObj_t> Figure::getPolygons() {
        return polygons_;
    }

    std::vector<TextureObj_t> Figure::getTextures(){
        return textures_;
    }

    matrix_t Figure::getMatrix() {
        return matrix_;
    }

    void Figure::setMatrix(matrix_t m){
        matrix_ = std::move(m);
        bounds = Matrix::getBounds(matrix_);
    }

    int Figure::getVerticesNum() {
        return matrix_.size();        
    }

    Edge_t Figure::getEdges() {
        return edges_;
    }

    int Figure::getNodesNum() const {
        if (!nodesNum.has_value()){
            std::set<Seg_t> unique_nodes;
            for (const auto& dots : polygons_) {
                for (size_t i = 1; i < dots.size(); ++i) {
                    unique_nodes.insert(Seg_t(dots[i-1].vi, dots[i].vi));
                }
            }
            *nodesNum = unique_nodes.size();
        }

        return *nodesNum;
    }

    Bounds Figure::getBounds() {
        return bounds;
    }
}
