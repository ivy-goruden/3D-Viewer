#include "controller.hpp"
#include "../include/obj_parser/obj_loader.hpp"
#include "../include/affine_transformer/transformer.hpp"

namespace s21{

    Controller::Controller(Figure figure) : figure_(figure){}

    Vert_t Controller::loadFigure(const char* filename){
        s21::ObjLoader loader = s21::ObjLoader();
        loader.loadObjFile(filename);
        s21::Figure figure  =s21::Figure(loader);
        figure_ = figure;
        Vert_t projection_ = s21::Transformer::getFigureProjection(figure_.getMatrix());
        return projection_;
    }

    Vert_t Controller::rotateFigure(double x, double y, double z){
        matrix_t rotate = s21::Transformer::Rotate(x,y,z, figure_.getMatrix());
        Vert_t projection_ = s21::Transformer::getFigureProjection(rotate);
        return projection_;
    }
}

