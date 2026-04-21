#ifndef SIMPLE_CANVAS_HPP
#define SIMPLE_CANVAS_HPP

#include <vector>
#include "include/main.h"
#include <gtk/gtk.h>
#include <cmath>
#include "controller/controller.hpp"

struct Point {
    double x, y;
    Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
};

class SimpleCanvas {
public:
    // Конструктор принимает GtkApplication
    SimpleCanvas(GtkWidget* drawing_area);
    ~SimpleCanvas();

    // Примитивы рисования
    void draw_line(cairo_t* cr, double x1, double y1, double x2, double y2);
    void draw_polygon(cairo_t* cr, const std::vector<Point>& points);
    void draw_dot(cairo_t* cr, double x, double y);

    void drawVert(cairo_t* cr);
    void loadFigure(const char* filename);
    void onDraw(cairo_t* cr, int width, int height);

    void setScale(double);
    void rotate(double x, double y, double z);
    void redraw();
    void toggleProjection();

private:
    GtkWidget* drawing_area;
    // Статические колбэки GTK
    static void on_draw_static(GtkDrawingArea* area, cairo_t* cr,
                               int width, int height, gpointer user_data);
    static void on_window_destroy(GtkWidget* widget, gpointer data);
    double scale_;
    double xStart_;
    double yStart_;
    double angleX_;
    double angleY_;
    double angleZ_;
    s21::Vert_t projection_;
    GtkWidget* widget_;
    s21::Controller* c_;

};

#endif // SIMPLE_CANVAS_HPP