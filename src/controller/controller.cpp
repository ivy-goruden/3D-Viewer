#include "controller.hpp"

#include <cmath>
#include <iostream>

#include "../include/affine_transformer/matrix.hpp"
#include "../include/affine_transformer/transformer.hpp"
#include "../include/obj_parser/obj_loader.hpp"

namespace s21 {

Controller::Controller() {
    angleX_ = 0;
    angleY_ = 0;
    angleZ_ = 0;
    camera_ = new Camera(0.0, 0.0, -10.0, 45, 10.0, 100.0);
    scale_ = 1;
    shiftx_ = 0;
    shifty_ = 0;
    diagonal_ = 0;

    prev_angleX_ = 0;
    prev_angleY_ = 0;
    prev_angleZ_ = 0;

    prev_shiftX_ = 0;
    prev_shiftY_ = 0;
    Poly_Proj_t projection_ = Poly_Proj_t();
}

Controller::~Controller() {
    delete figure_;
    delete camera_;
}

float Controller::getAngleX() { return angleX_; }

float Controller::getAngleY() { return angleY_; }

float Controller::getAngleZ() { return angleZ_; }

void Controller::setAngleX(int angle) {
    prev_angleX_ = angleX_;
    angleX_ = angle;
}

void Controller::setAngleY(int angle) {
    prev_angleY_ = angleY_;
    angleY_ = angle;
}

void Controller::setAngleZ(int angle) {
    prev_angleZ_ = angleZ_;
    angleZ_ = angle;
}

void Controller::setCameraZ(float z) { camera_->z = z; }

float Controller::getScale() { return scale_; }

float Controller::getCameraZ() { return camera_->z; }

void Controller::shiftX(int shiftx) {
    prev_shiftX_ = shiftx_;
    shiftx_ = shiftx;
}

void Controller::shiftY(int shifty) {
    prev_shiftY_ = shifty_;
    shifty_ = shifty;
}

void Controller::loadFigure(const char* filename, int canvasW, int canvasH) {
    s21::ObjLoader loader = s21::ObjLoader();
    loader.loadObjFile(filename);
    s21::Figure* figure = new s21::Figure(loader);
    figure_ = figure;

    s21::matrix_t shapeVert = figure_->getMatrix();
    s21::Bounds b = figure_->getBounds();
    matrix_t fig =
        s21::Transformer::Translate(-(b.maxx + b.minx) / 2, -(b.maxy + b.miny) / 2, -(b.maxz + b.minz) / 2, shapeVert);
    figure->setMatrix(fig);

    FigureView_Sett(canvasW, canvasH);
}

void Controller::FigureView_Sett(int canvasW, int canvasH) {
    // recount bounds
    Bounds b = figure_->getBounds();
    int x = b.maxx - b.minx;
    int y = b.maxy - b.miny;
    int z = b.maxz - b.minz;
    diagonal_ = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2)) + 10;
    camera_->z = (diagonal_ + camera_->d) * -1;
    scale_ = 0.8 * std::min(canvasW, canvasH) / diagonal_;
}

Poly_Proj_t Controller::getFigure() {
    if (figure_ != nullptr) {
        if (prev_angleX_ != angleX_ || prev_angleY_ != angleY_ || prev_angleZ_ != angleZ_) {
            matrix_t rotate = s21::Transformer::Rotate(angleX_ - prev_angleX_, angleY_ - prev_angleY_,
                                                       angleZ_ - prev_angleZ_, figure_->getMatrix());
            prev_angleX_ = angleX_;
            prev_angleY_ = angleY_;
            prev_angleZ_ = angleZ_;
            figure_->setMatrix(rotate);
        }
        if (prev_shiftX_ != shiftx_ || prev_shiftY_ != shifty_) {
            matrix_t translate =
                s21::Transformer::Translate(shiftx_ - prev_shiftX_, shifty_ - prev_shiftY_, 0, figure_->getMatrix());
            figure_->setMatrix(translate);
        }
        projection_ = getFigureProjection(figure_->getMatrix());
    }
    return projection_;
}

Bounds Controller::getFigureBounds() {
    Bounds bounds = {0, 0, 0, 0, 0, 0};
    if (figure_ != nullptr) {
        bounds = figure_->getBounds();
    }
    return bounds;
}

Poly_Proj_t Controller::getFigureProjection(const matrix_t original) {
    Poly_t poly = figure_->getPolygons();
    if (parallel_projection_) {
        return s21::Transformer::getParallelProjection(poly, original, scale_);
    }
    return s21::Transformer::getPerspectiveProjection(poly, original, *camera_, scale_);
}

void Controller::toggleProjection() {
    parallel_projection_ = !parallel_projection_;
    if (parallel_projection_) {
        scale_ = 1 / 2;
    } else {
        scale_ = diagonal_;
    }
}

matrix_t Controller::getMatrix() {
    matrix_t m;
    if (figure_ != nullptr) {
        m = figure_->getMatrix();
    }
    return m;
}

Edge_t Controller::getEdges() {
    Edge_t edges;
    if (figure_ != nullptr) {
        edges = figure_->getEdges();
    }
    return edges;
}

Poly_t Controller::getPolygons() {
    Poly_t polygon;
    if (figure_ != nullptr) {
        polygon = figure_->getPolygons();
    }
    return polygon;
}

int Controller::getEdgesNum() {
    int edges = 0;
    if (figure_ != nullptr) {
        edges = figure_->getNodesNum();
    }
    return edges;
}

int Controller::getVerticesNum() {
    int vert = 0;
    if (figure_ != nullptr) {
        vert = figure_->getVerticesNum();
    }
    return vert;
}

void Controller::setScale(float scale) { scale_ = scale; }

}  // namespace s21

matrix_t Controller::getMVP(){
    matrix_t orig = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
    matrix_t rotate = Transformer::rotate(angleX_, angleY_, angleZ_, orig);
    matrix_t scale = Transformer::scale(scale_, scale_, scale_, rotate);
    matrix_t translate = Transformer::Translate(shiftx_, shiftx_, 0, scale);
    return translate;
}
