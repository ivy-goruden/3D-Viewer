#include "controller.hpp"
#include "../include/obj_parser/obj_loader.hpp"
#include "../include/affine_transformer/transformer.hpp"

namespace s21{

    Controller::Controller(){}

    Vert_t Controller::loadFigure(const char* filename){
        s21::ObjLoader loader = s21::ObjLoader();
        loader.loadObjFile(filename);
        s21::Figure *figure  = new s21::Figure(loader);
        figure_ = figure;
        Vert_t projection_ = getFigureProjection(figure_->getMatrix());
        return projection_;
    }

    Vert_t Controller::rotateFigure(double x, double y, double z){
        Vert_t projection_ = Vert_t();
        if (figure_ != nullptr){
            matrix_t rotate = s21::Transformer::Rotate(x,y,z, figure_->getMatrix());
            projection_ = getFigureProjection(rotate);
            
        }
        return projection_;

    }

    Vert_t Controller::getFigureProjection(const matrix_t original){
        if (parallel_projection_){
            return s21::Transformer::getParallelProjection(original);
        }
        return s21::Transformer::getPerspectiveProjection(original);
    }
}

