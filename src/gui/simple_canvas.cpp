#include "simple_canvas.hpp"
#include <iostream>
#include <format>
#include "../include/globals.h"
#include "../include/projection/projection.hpp"
#include "../include/axes.hpp"

void drawSomething(cairo_t* cr, SimpleCanvas& cv) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, cv.lineWidth_ * 10);
    cairo_arc(cr, 0, 0, 100, 0, 2 * M_PI);
    cairo_stroke(cr);
}

// Реализация методов класса SimpleCanvas для GTK4

SimpleCanvas::SimpleCanvas(GtkWidget* drawing_area) {

    // Устанавливаем функцию рисования (вместо сигнала "draw")
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area),
                                   on_draw_static,
                                   this, nullptr);
    scale_ = 1;
    xStart_ = 0;
    yStart_ = 0;
    widget_ = drawing_area;
    c_ = new s21::Controller();
    dotColor_ = Rgb(1,0,0,1);
    lineColor_ = Rgb(0,1,0,1);
    polyColor_ = Rgb(0,0,1,1);
    bgColor_ = Rgb(1,1,1,1);
    fillPoly_ = false;
    lineWidth_ = 0.01;
    vertWidth_ = 0.1;
    vertType_ = None;
    lineType_ = Solid;

    gridProjection_ = c_->gridProjection();
}

SimpleCanvas::~SimpleCanvas() {
    // Деструктор (можно оставить пустым или освободить ресурсы)
}

void SimpleCanvas::draw_line(cairo_t* cr, double x1, double y1, double x2, double y2) {
    cairo_set_source_rgb(cr, lineColor_.red, lineColor_.green, lineColor_.blue);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    if (lineType_ == Dotted){
        double dashes[] = {lineWidth_*2, lineWidth_};
        int num_dashes = 2;
        double offset = 0.0;
        cairo_set_dash(cr, dashes, num_dashes, offset);
    }

    cairo_stroke(cr);
}

void SimpleCanvas::draw_polygon(cairo_t* cr, const std::vector<s21::Point>& points) {
    if (points.size() < 3) return;

    cairo_set_source_rgb(cr, polyColor_.red, polyColor_.green, polyColor_.blue);

    cairo_move_to(cr, points[0].x, points[0].y);
    for (size_t i = 1; i < points.size(); ++i) {
        cairo_line_to(cr, points[i].x, points[i].y);
    }
    cairo_close_path(cr);
    if (fillPoly_){
        cairo_fill(cr);
    }else{
        cairo_stroke(cr);
    }

}

void SimpleCanvas::draw_dot(cairo_t* cr, double x, double y) {
    switch(vertType_){
        case Circle:
            cairo_arc(cr, x, y, lineWidth_*2, 0, 2 * M_PI);
        case Rect:
            double d = lineWidth_*2*std::sqrt(2)/2;
            cairo_rectangle (cr, x-d, y-d, 2*d, 2*d);
    }
    cairo_set_source_rgb(cr, dotColor_.red, dotColor_.green, dotColor_.blue);
    cairo_fill(cr);
}

void SimpleCanvas::drawAxes(cairo_t* cr) {
    std::cout << "[SimpleCanvas::drawAxes]\n";
    s21::Axes axes;
    for (const auto& i : axes.getMatrix()) {
        std::cout << std::format("{},{},{}\n", i[0], i[1], i[2]);
    }
    s21::matrix_t m = s21::Transformer::Rotate(c_->getAngleX(), c_->getAngleY(), c_->getAngleZ(), axes.getMatrix());
    for (const auto& i : m) {
        std::cout << std::format("{},{},{}\n", round(i[0]), round(i[1]), round(i[2]));
    }

    s21::Vert_t projection;
    s21::SimplePerspective proj(10, 1920, 1080);
    for (const auto& point3d : m) {
        s21::Point p = proj.project({point3d[0], point3d[1], point3d[2]});
        projection.push_back(p);
        std::cout << std::format("{},{}\n", p.x, p.y);
    }

    cairo_set_line_width(cr, lineWidth_ * 100);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_move_to(cr, projection[0].x, projection[0].y);
    cairo_line_to(cr, projection[1].x, projection[1].y);
    cairo_stroke(cr);
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_move_to(cr, projection[2].x, projection[2].y);
    cairo_line_to(cr, projection[3].x, projection[3].y);
    cairo_stroke(cr);
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_move_to(cr, projection[4].x, projection[4].y);
    cairo_line_to(cr, projection[5].x, projection[5].y);
    cairo_stroke(cr);
}

void SimpleCanvas::drawGrid(cairo_t* cr) {
    std::cout << "[SimpleCanvas::drawGrid]\n";
    cairo_set_source_rgb(cr, 0, 0, 0);
    for (const auto& point : gridProjection_) {
        // std::cout << std::format("x{},y{}\n", point.x, point.y);
        cairo_arc(cr, point.x, point.y, 2, 0, 2 * M_PI);
        cairo_set_line_width(cr, lineWidth_ * 1);
        cairo_fill(cr);
    }
}

void SimpleCanvas::drawVert(cairo_t* cr) {
    for (const auto& point : projection_) {
        draw_dot(cr, point.x, point.y);
    }
}

void SimpleCanvas::drawEdges(cairo_t* cr){
    s21::Edge_t edges = c_->getEdges();
    for (s21::Seg_t s: edges){
        draw_line(cr, projection_[s.start].x, projection_[s.start].y, projection_[s.end].x, projection_[s.end].y);

    }
}

void SimpleCanvas::drawFaces(cairo_t* cr){
    s21::Poly_t poly = c_->getPolygons();
    for(auto dots: poly){
        std::vector<s21::Point> points;
        for(int n: dots){
            points.push_back(projection_[n]);
        }
        draw_polygon(cr, points);
    }
}

void SimpleCanvas::on_draw_static(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
    SimpleCanvas* self = static_cast<SimpleCanvas*>(user_data);
    self->onDraw(cr, width, height);
}

void SimpleCanvas::loadFigure(const char* filename) {
    projection_ = c_->loadFigure(filename);    
}

void SimpleCanvas::redraw(){
    gtk_widget_queue_draw(GTK_WIDGET(widget_));
}

void SimpleCanvas::onDraw(cairo_t* cr, int width, int height) {
    cairo_set_source_rgb(cr, bgColor_.red, bgColor_.green, bgColor_.blue);
    cairo_paint(cr);
    cairo_set_line_width(cr, lineWidth_);
    cairo_translate(cr, width / 2 + xStart_, height / 2 + yStart_);
    cairo_scale(cr, scale_, scale_);

    // drawGrid(cr);
    drawAxes(cr);
    // drawFaces(cr);
    // drawEdges(cr);
    // drawVert(cr);

    drawSomething(cr, *this);
    std::cout << "[SimpleCanvas::onDraw] " << std::format("w{},h{},xc{},yc{},s{},lw{}\n", width, height, width/2+xStart_, height/2+yStart_, scale_, lineWidth_);
}

void SimpleCanvas::setScale(double scale){
    scale_ = scale;
}

void SimpleCanvas::setPosX(double x){
    xStart_ = x;
}

void SimpleCanvas::setPosY(double y){
    yStart_ = y;
}

void SimpleCanvas::rotateAbs(double x, double y, double z){
    c_->setAngleX(x);
    c_->setAngleY(y);
    c_->setAngleZ(z);
    projection_ = c_->getFigure();
    gridProjection_ = c_->gridProjection();
}

void SimpleCanvas::rotateX(double x){
    c_->setAngleX(x);
    projection_ = c_->getFigure();
    gridProjection_ = c_->gridProjection();
}

void SimpleCanvas::rotateY(double y){
    c_->setAngleY(y);
    projection_ = c_->getFigure();
    gridProjection_ = c_->gridProjection();
}

void SimpleCanvas::rotateZ(double z){
    c_->setAngleY(z);
    projection_ = c_->getFigure();
    gridProjection_ = c_->gridProjection();
}

void SimpleCanvas::toggleProjection(){
    c_->toggleProjection();
    projection_ = c_->getFigure();
}

void SimpleCanvas::togglePolyFill(){
    fillPoly_ = !fillPoly_;
}

void SimpleCanvas::setVertType(int type){
    switch(type){
        case 1:
            vertType_ = Shape(Circle);
            break;
        case 2:
            vertType_ = Shape(Rect);
            break;
        default:
            vertType_ = Shape(None);
            break;
    }
}

void SimpleCanvas::setLineType(int type){
    switch(type){
        case 1:
            lineType_ = Stroke(Dotted);
            break;
        default:
            lineType_ = Stroke(Solid);
            break;
    }
}

void SimpleCanvas::setBgColor(Rgb color){
    bgColor_ = color;
}
void SimpleCanvas::setVertColor(Rgb color){
    dotColor_ = color;
}

void SimpleCanvas::setLineWidth(double width){
    lineWidth_ = width;
}

void SimpleCanvas::setZoom(double zoom) {
    c_->setScale(zoom);
    projection_ = c_->getFigure();
}

double SimpleCanvas::getAngleX() {
    return c_->getAngleX();
}

double SimpleCanvas::getAngleY() {
    return c_->getAngleY();
}

double SimpleCanvas::getAngleZ() {
    return c_->getAngleZ();
}

int SimpleCanvas::getEdgesNum(){
    return c_->getEdgesNum();
}

int SimpleCanvas::getVerticesNum(){
    return c_->getVerticesNum();
}

s21::Point SimpleCanvas::toScreenPoint(s21::Point canvasPoint) {
    return s21::Point{window_w / 2 + canvasPoint.x, window_h / 2 - canvasPoint.y};
}

s21::Point SimpleCanvas::toCanvasPoint(s21::Point screenPoint) {
    return s21::Point{-window_w / 2 + screenPoint.x, window_h / 2 - screenPoint.y};
}

// void SimpleCanvas::renderSegment(const s21::Point3d& p1, const s21::Point3d& p2) {
//     // Настройка камеры
//     s21::Point3d cameraPos = {0, 0, 10};     // камера на оси Z
//     s21::Point3d lookAt = {0, 0, 0};          // смотрим в начало координат
//     s21::Point3d upVec = {0, 1, 0};           // Y вверх
    
//     PerspectiveProjection proj(
//         cameraPos, lookAt, upVec,
//         60.0,      // FOV 60 градусов
//         16.0/9.0,  // aspect ratio
//         0.1, 100.0,// near, far
//         1920, 1080 // разрешение экрана
//     );
    
//     auto [sp1, sp2] = proj.projectSegment(p1, p2);
    
//     // Рисуем отрезок на экране от sp1 до sp2
//     drawLine(sp1.x, sp1.y, sp2.x, sp2.y);
// }

