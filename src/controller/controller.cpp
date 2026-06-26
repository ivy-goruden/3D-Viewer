#include "controller.hpp"

#include <cmath>
#include <iostream>

#include "../include/affine_transformer/matrix.hpp"
#include "../include/figure/figure.hpp"
#include "../include/affine_transformer/transformer.hpp"
#include "../include/obj_parser/obj_loader.hpp"

namespace s21 {

Controller::Controller() {
    angleX_ = 0;
    angleY_ = 0;
    angleZ_ = 0;
    camera_ = new Camera(0.0, 0.0, 12.0, 45, 10.0, 100.0);
    scale_ = 1;
    shiftx_ = 0;
    shifty_ = 0;
    diagonal_ = 0;
    Poly_Proj_t projection_ = Poly_Proj_t();
    parallel_projection_ = true;
}

Controller::~Controller() {
    delete figure_;
    delete camera_;
}

float Controller::getAngleX() { return angleX_; }

float Controller::getAngleY() { return angleY_; }

float Controller::getAngleZ() { return angleZ_; }

void Controller::setAngleX(int angle) {
    angleX_ = angle;
}

void Controller::setAngleY(int angle) {
    angleY_ = angle;
}

void Controller::setAngleZ(int angle) {
    angleZ_ = angle;
}

void Controller::setCameraZ(float z) { camera_->z = z; }

float Controller::getScale() { return scale_; }

float Controller::getCameraZ() { return camera_->z; }

void Controller::shiftX(int shiftx) {
    shiftx_ = shiftx;
}

void Controller::shiftY(int shifty) {
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
    diagonal_ = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
    camera_->z = (b.maxz + camera_->d);
    scale_ = 0.8 * std::min(canvasW, canvasH) / diagonal_;
    camera_->far = (camera_->z - diagonal_)*2;
}

// Poly_Proj_t Controller::getFigure() {
//     if (figure_ != nullptr) {
//         matrix_t rotate = Transformer::Rotate(angleX_, angleY_, angleZ_, figure_->getMatrix());
//         matrix_t scale = Transformer::Scale(scale_, scale_, scale_, rotate);
//         matrix_t translate = Transformer::Translate(shiftx_, shiftx_, 0, scale);
//         projection_ = getFigureProjection(translate);
//     }
//     return projection_;
// }

Bounds Controller::getFigureBounds() {
    Bounds bounds = {0, 0, 0, 0, 0, 0};
    if (figure_ != nullptr) {
        bounds = figure_->getBounds();
    }
    return bounds;
}

// Poly_Proj_t Controller::getFigureProjection(const matrix_t original) {
//     Poly_t poly = figure_->getPolygons();
//     if (parallel_projection_) {
//         return s21::Transformer::getParallelProjection(poly, original, scale_);
//     }
//     return s21::Transformer::getPerspectiveProjection(poly, original, *camera_, scale_);
// }

void Controller::toggleProjection() {
    parallel_projection_ = !parallel_projection_;
    // if (parallel_projection_) {
    //     scale_ = 1 / 2;
    // } else {
    //     scale_ = diagonal_;
    // }
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

std::vector<s21::FaceObj_t> Controller::getPolygons() {
    std::vector<s21::FaceObj_t> polygon;
    if (figure_ != nullptr) {
        polygon = figure_->getPolygons();
    }
    return polygon;
}

std::vector<TextureObj_t> Controller::getTextures(){
    std::vector<TextureObj_t> texture;
    if (figure_ != nullptr) {
        texture = figure_->getTextures();
    }
    return texture;
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



matrix_t Controller::getViewMatrix(){
    return Transformer::getViewMatrix(camera_, Point3d(0,0,0));
}

matrix_t Controller::getProjMatrix(float aspect){
    if (parallel_projection_){
        return Transformer::getPerspMatrix(camera_, aspect);
    }else{
        return Transformer::getOrthoMatrix(camera_, aspect);
    }
}

matrix_t Controller::getModelMatrix(){
    matrix_t orig = {{1,0,0,0}, {0,1,0,0},{0,0,1,0},{0,0,0,1}};
    matrix_t rotate = Transformer::Rotate(angleX_, angleY_, angleZ_, orig);
    matrix_t scale = Transformer::Scale(scale_, scale_, scale_, rotate);
    matrix_t translate = Transformer::Translate(shiftx_, shiftx_, 0, scale);
    return translate;
}

matrix_t Controller::getMVP(float aspect){
    matrix_t mat = getModelMatrix();
    matrix_t mvp = Transformer::setView(&mat, camera_, Point3d(0,0,0));
    if (parallel_projection_){
        mvp = Transformer::perspective(&mvp, camera_, aspect);
    }else{
        mvp = Transformer::ortho(&mvp, camera_, aspect);
    }
    return mvp;
}

std::vector<NormalObj_t> Controller::getNormals(){
    return figure_->getNormals();
}

}  // namespace s21