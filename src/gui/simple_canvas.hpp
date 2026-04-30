#ifndef SIMPLE_CANVAS_HPP
#define SIMPLE_CANVAS_HPP

#include <vector>
#include "../include/main.h"
#include <gtk/gtk.h>
#include <cmath>
#include "../controller/controller.hpp"
#include "gui_globals.h"

class SimpleCanvas {
public:
    // Конструктор принимает GtkApplication
    SimpleCanvas(GtkWidget* drawing_area);
    ~SimpleCanvas();
    void onDraw(cairo_t* cr, int width, int height);
    void redraw();

    // Примитивы рисования
    void draw_line(cairo_t* cr, double x1, double y1, double x2, double y2);
    void draw_polygon(cairo_t* cr, const std::vector<s21::Point>& points);
    void draw_dot(cairo_t* cr, double x, double y);

    // Отрисовка фигуры
    void drawVert(cairo_t* cr);
    void drawEdges(cairo_t* cr);
    void drawFaces(cairo_t* cr);
    
    // Элементы управления
    void loadFigure(const char* filename);
    void setScale(double);
    void setPosX(double);
    void setPosY(double);
    void toggleProjection();
    void rotateX(double);
    void rotateY(double);
    void rotateZ(double);
    void rotateAbs(double x, double y, double z);
    void togglePolyFill();
    void setVertType(int type);
    void setLineType(int type);
    void setBgColor(Rgb color);
    double getAngleX();
    double getAngleY();
    double getAngleZ();

private:
    GtkWidget* drawing_area;
    // Статические колбэки GTK
    static void on_draw_static(GtkDrawingArea* area, cairo_t* cr,
                               int width, int height, gpointer user_data);
    static void on_window_destroy(GtkWidget* widget, gpointer data);
    double scale_;
    double xStart_;
    double yStart_;
    s21::Vert_t projection_;
    GtkWidget* widget_;
    s21::Controller* c_;
    double lineWidth_;
    double vertWidth_;

    Rgb dotColor_;
    Rgb lineColor_;
    Rgb polyColor_;
    Rgb bgColor_;

    bool fillPoly_;
    Shape vertType_;
    Stroke lineType_;
};

#endif // SIMPLE_CANVAS_HPP
