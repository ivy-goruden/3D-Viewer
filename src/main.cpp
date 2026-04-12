#include "include/globals.h"
#include "simple_canvas.hpp"
int main(int argc, char* argv[]){
    matrix_t fMatrix = {{0.0,1.0,0.0,1.0},{0.0,0.0,0.0,1.0},{1.0,0.0,0.0,1.0}};
    Node_t nodes = {std::make_pair(0,1), std::make_pair(1,2)};
    Poly_t polies = {{0,1,2}};
    Figure figure = Figure(fMatrix, polies, nodes)
    Vert_t projection = Transformer::getFigureProjection(figure.getMatrix())
    
    
    gtk_init(&argc, &argv);
    SimpleCanvas canvas;
    gtk_main();
    return 0;
}