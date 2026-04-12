#include "simple_canvas.hpp"
#include "include/main.h"
#include <gtk/gtk.h>
#include <cmath>
// Реализация методов класса SimpleCanvas для GTK4

SimpleCanvas::SimpleCanvas(GtkApplication* app) {
    // Создаём окно
    GtkWidget* window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Algorithm Testing Canvas");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_application(GTK_WINDOW(window), app);

    // Создаём область рисования
    drawing_area = gtk_drawing_area_new();
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);

    // Устанавливаем функцию рисования (вместо сигнала "draw")
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area),
                                   SimpleCanvas::on_draw_static,
                                   this, nullptr);

    // Подключаем сигнал закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), nullptr);

    gtk_window_present(GTK_WINDOW(window));
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

void SimpleCanvas::my_algorithm(cairo_t* cr) {
    // Пример отрисовки: точки, линии, полигон
    draw_dot(cr, 100, 100);
    draw_dot(cr, 200, 150);
    draw_dot(cr, 300, 200);

    draw_line(cr, 100, 100, 400, 100);
    draw_line(cr, 400, 100, 400, 300);

    std::vector<Point> polygon;
    polygon.emplace_back(500, 100);
    polygon.emplace_back(600, 150);
    polygon.emplace_back(550, 250);
    polygon.emplace_back(450, 200);
    draw_polygon(cr, polygon);
    s21::matrix_t fMatrix = {{0.0,1.0,0.0,1.0},{0.0,0.0,0.0,1.0},{1.0,0.0,0.0,1.0}};
    s21::Poly_t polies = {{0,1,2}};
    s21::Figure figure = s21::Figure(fMatrix, polies);
    s21::Vert_t projection = s21::Transformer::getFigureProjection(figure.getMatrix());
    

    // Здесь можно добавить свои тестовые фигуры
}

// Статическая функция отрисовки для GTK4
void SimpleCanvas::on_draw_static(GtkDrawingArea* area, cairo_t* cr,
                                  int width, int height, gpointer user_data) {
    SimpleCanvas* canvas = static_cast<SimpleCanvas*>(user_data);

    // Белый фон
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Вызов алгоритма пользователя
    canvas->my_algorithm(cr);
}

// Обработчик закрытия окна
void SimpleCanvas::on_window_destroy(GtkWidget* widget, gpointer data) {
    GApplication* app = g_application_get_default();
    if (app) {
        g_application_quit(app);
    }
}