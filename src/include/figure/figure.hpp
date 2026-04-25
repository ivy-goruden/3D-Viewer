#ifndef FIGURE
#define FIGURE

#include "../globals.h"
#include "../obj_parser/obj_loader.hpp"
#include <set>
#include <algorithm>

namespace s21{

class Figure{
    protected:
        Poly_t polygons_;               //list of iterators for the vertices connected in polys
        Edge_t edges_;                  //pairs of iterators of the connected vertices
        matrix_t matrix_;               //matrix of original figure for transformation
        Vert_t projectionVertices_;    //transformed projection of original figure

        void Unique(Edge_t&);

    
    public:
        Figure(matrix_t, Poly_t, Edge_t);
        Figure(ObjLoader);
        Poly_t getPolygons();
        Edge_t getEdges();
        matrix_t getMatrix();
        int getVerticesNum();
        int getNodesNum();
        Vert_t getProjection();
};

}

#endif
