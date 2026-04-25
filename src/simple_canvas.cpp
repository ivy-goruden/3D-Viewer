#include "simple_canvas.hpp"

// Реализация методов класса SimpleCanvas для GTK4

SimpleCanvas::SimpleCanvas(GtkWidget* drawing_area) {

    // Устанавливаем функцию рисования (вместо сигнала "draw")
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area),
                                   on_draw_static,
                                   this, nullptr);

    scale_ = 100;
    xStart_ = 50;
    yStart_ = 50;
    angleX_ = X2_d;
    angleY_ = Y2_d;
    angleZ_ = Z2_d;
    widget_ = drawing_area;
    c_ = new s21::Controller();
}

SimpleCanvas::~SimpleCanvas() {
    // Деструктор (можно оставить пустым или освободить ресурсы)
}

void SimpleCanvas::draw_line(cairo_t* cr, double x1, double y1, double x2, double y2) {
    cairo_set_source_rgb(cr, 0, 1, 0); // Зелёный
    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);
}

void SimpleCanvas::draw_polygon(cairo_t* cr, const std::vector<Point>& points) {
    if (points.size() < 3) return;

    cairo_set_source_rgb(cr, 1, 0, 0); // Красный
    cairo_set_line_width(cr, 2);

    cairo_move_to(cr, points[0].x, points[0].y);
    for (size_t i = 1; i < points.size(); ++i) {
        cairo_line_to(cr, points[i].x, points[i].y);
    }
    cairo_close_path(cr);
    cairo_stroke(cr);
}

void SimpleCanvas::draw_dot(cairo_t* cr, double x, double y) {
    cairo_arc(cr, x, y, 5, 0, 2 * M_PI);
    cairo_set_source_rgb(cr, 0, 0, 1); // Синий
    cairo_fill(cr);
}

void SimpleCanvas::drawVert(cairo_t* cr){
    for (const auto& point : projection_) {
        cairo_new_path(cr);
        cairo_arc(cr, point.x, point.y, 0.1, 0, 2 * M_PI);
        cairo_fill(cr);
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
    // Очистка фона (белый)
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    
    // Центрируем систему координат
    cairo_translate(cr, xStart_, yStart_);
    // Масштабируем (подберите под свой размер фигуры)
    cairo_scale(cr, scale_, scale_);
    
    // Рисуем точки из проекции
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // красный

    drawVert(cr);

}

void SimpleCanvas::setScale(double scale){
    scale_ = scale;
}

void SimpleCanvas::rotate(double x, double y, double z){
    projection_ = c_->rotateFigure(x,y,z);
}

void SimpleCanvas::toggleProjection(){
    projection_ = c_->toggleProjection();
}