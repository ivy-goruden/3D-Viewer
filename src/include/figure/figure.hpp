#ifndef FIGURE
#define FIGURE

#include "../globals.h"
#include "../obj_parser/obj_loader.hpp"

namespace s21{

class Figure{
    protected:
        Poly_t polygons_;               //list of iterators for the vertices connected in polys
        Node_t nodes_;                  //pairs of iterators of the connected vertices
        matrix_t matrix_;               //matrix of original figure for transformation
        Vert_t projectionVertices_;    //transformed projection of original figure

    
    public:
        Figure(matrix_t, Poly_t, Node_t);
        Figure(ObjLoader);
        Poly_t getPolygons();
        Node_t getNodes();
        matrix_t getMatrix();
        int getVerticesNum();
        int getNodesNum();
        Vert_t getProjection();
};

}

#endif
