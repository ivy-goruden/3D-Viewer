#ifndef FIGURE
#define FIGURE

#include "../globals.h"
#include "../obj_parser/obj_loader.hpp"
#include <set>
#include <algorithm>
#include <optional>

namespace s21{

class Figure{
    protected:
        std::vector<FaceObj_t> polygons_;            //list of iterators for the vertices and textures connected in polys
        Edge_t edges_;                  //pairs of iterators of the connected vertices
        matrix_t matrix_;               //matrix of original figure for transformation
        Vert_t projectionVertices_;     //transformed projection of original figure
        std::vector<TextureObj_t> textures_; //textures
        float minz;
        Bounds bounds;
        void Unique(Edge_t&) const;
        mutable std::optional<int> nodesNum;
    
    public:
        Figure(matrix_t&, Poly_t, Edge_t);
        Figure(ObjLoader);
        std::vector<FaceObj_t> getPolygons();
        Edge_t getEdges();
        matrix_t getMatrix();
        void setMatrix(matrix_t);
        int getVerticesNum();
        int getNodesNum() const;
        Vert_t getProjection();
        Bounds getBounds();
        std::vector<TextureObj_t> getTextures();
};

}

#endif
