#include "controller.hpp"
#include "../include/obj_parser/obj_loader.hpp"
#include "../include/affine_transformer/transformer.hpp"
#include <cmath>

namespace s21{

    Controller::Controller() {
        angleX_ = X2_d;
        angleY_ = Y2_d;
        angleZ_ = Z2_d;
        camera_ = 3;
    }

    double Controller::getAngleX() {
        return angleX_;
    }

    double Controller::getAngleY() {
        return angleY_;
    }

    double Controller::getAngleZ() {
        return angleZ_;
    }

    void Controller::setAngleX(int angle){
        angleX_ = angle;
    }
    void Controller::setAngleY(int angle){
        angleY_ = angle;
    }
    void Controller::setAngleZ(int angle){
        angleZ_ = angle;
    }

    Vert_t Controller::loadFigure(const char* filename){
        s21::ObjLoader loader = s21::ObjLoader();
        loader.loadObjFile(filename);
        s21::Figure *figure  = new s21::Figure(loader);
        figure_ = figure;
        Vert_t projection_ = getFigureProjection(figure_->getMatrix());
        return projection_;
    }

    Vert_t Controller::getFigure(){
        Vert_t projection_ = Vert_t();
        if (figure_ != nullptr){
            matrix_t rotate = s21::Transformer::Rotate(angleX_,angleY_,angleZ_, figure_->getMatrix());
            projection_ = getFigureProjection(rotate);
            
        }
        return projection_;

    }

    Vert_t Controller::getFigureProjection(const matrix_t original){
        if (parallel_projection_){
            return s21::Transformer::getParallelProjection(original, scale_);
        }
        return s21::Transformer::getPerspectiveProjection(original, camera_, scale_);
    }

    void Controller::toggleProjection(){
        parallel_projection_ = !parallel_projection_;
    }

    Edge_t Controller::getEdges(){
        Edge_t edges;
        if (figure_ != nullptr){
            edges = figure_->getEdges();
        }
        return edges;
    }

    Poly_t Controller::getPolygons(){
        Poly_t polygon;
        if (figure_ != nullptr){
            polygon = figure_->getPolygons();
        }
        return polygon;
    }

    int Controller::getEdgesNum(){
        int edges = 0;
        if (figure_ != nullptr){
            edges = figure_->getNodesNum();
        }
        return edges;
    }

    int Controller::getVerticesNum(){
        int vert = 0;
        if (figure_ != nullptr){
            vert = figure_->getVerticesNum();
        }
        return vert;
    }

    void Controller::setScale(double scale){
        scale_ = scale;
    }
    
}

