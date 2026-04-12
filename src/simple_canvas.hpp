#ifndef SIMPLE_CANVAS_HPP
#define SIMPLE_CANVAS_HPP

#include <gtk/gtk.h>
#include <vector>
#include <cmath>

struct Point {
    double x, y;
    Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
};

class SimpleCanvas {
public:
    // Конструктор принимает GtkApplication
    SimpleCanvas(GtkApplication* app);
    ~SimpleCanvas();

    // Примитивы рисования
    void draw_line(cairo_t* cr, double x1, double y1, double x2, double y2);
    void draw_polygon(cairo_t* cr, const std::vector<Point>& points);
    void draw_dot(cairo_t* cr, double x, double y);

    // Ваш алгоритм (реализуйте в .cpp)
    void my_algorithm(cairo_t* cr);

private:
    GtkWidget* drawing_area;
    // Статические колбэки GTK
    static void on_draw_static(GtkDrawingArea* area, cairo_t* cr,
                               int width, int height, gpointer user_data);
    static void on_window_destroy(GtkWidget* widget, gpointer data);
    double scale_;
    double xStart_;
    double yStart_;

};

#endif // SIMPLE_CANVAS_HPP