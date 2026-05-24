#include "controller.hpp"
#include "../include/obj_parser/obj_loader.hpp"
#include "../include/affine_transformer/transformer.hpp"
#include "../include/affine_transformer/matrix.hpp"
#include <cmath>
#include <iostream>

namespace s21{

    Controller::Controller() {
        angleX_ = 0;
        angleY_ = 0;
        angleZ_ = 0;
        camera_ = 10;
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

    void Controller::setCamera(int camera){
        camera_ = camera;
    }

    double Controller::getScale(){
        return scale_;
    }
    
    int Controller::getZoom(){
        return camera_;
    }

    void Controller::shiftX(int shiftx){
        shiftx_ = shiftx;
    }

   void Controller::shiftY(int shifty){
        shifty_ = shifty;
    }

    Vert_t Controller::loadFigure(const char* filename, int canvasW, int canvasH){
        s21::ObjLoader loader = s21::ObjLoader();
        loader.loadObjFile(filename);
        s21::Figure *figure  = new s21::Figure(loader);
        figure_ = figure;

        s21::matrix_t shapeVert = figure_->getMatrix();
        s21::Bounds b = figure_->getBounds();
        matrix_t fig = s21::Transformer::Translate(-(b.maxx + b.minx)/2, -(b.maxy + b.miny)/2, -(b.maxz + b.minz)/2, shapeVert);
        figure->setMatrix(fig);

        int x = b.maxx - b.minx;
        int y = b.maxy - b.miny;
        int z = b.maxz - b.minz;        
        diagonal_ =  std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2))+10;
        //recount bounds
        b = figure_->getBounds();
        Vert_t projection_ = getFigureProjection(fig, b.minz);

        camera_ = diagonal_;
        scale_ = camera_;
        return projection_;
    }

    Vert_t Controller::getFigure(){
        Vert_t projection_ = Vert_t();
        if (figure_ != nullptr){
            matrix_t rotate = s21::Transformer::Rotate(angleX_,angleY_,angleZ_, figure_->getMatrix());
            matrix_t translate = s21::Transformer::Translate(shiftx_, shifty_, 0, rotate);
            s21::Bounds bn = s21::Matrix::getBounds(translate);
            projection_ = getFigureProjection(translate, bn.minz);
        }
        return projection_;
    }

    Bounds Controller::getFigureBounds(){
        Bounds bounds = Bounds{};
        if (figure_ != nullptr){
            bounds = figure_->getBounds();
        }
        return bounds;
    }

    Vert_t Controller::getFigureProjection(const matrix_t original, double minz) {
        if (parallel_projection_){
            return s21::Transformer::getParallelProjection(original, scale_);
        }
        return s21::Transformer::getPerspectiveProjection(original, camera_, scale_, minz);
    }

    void Controller::toggleProjection(){
        parallel_projection_ = !parallel_projection_;
        if (parallel_projection_){
            scale_ = 1/2;
        }
        else{
            scale_ = diagonal_;
        }

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

