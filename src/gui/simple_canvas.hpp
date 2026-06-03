#ifndef SIMPLE_CANVAS_HPP
#define SIMPLE_CANVAS_HPP

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <cmath>
#include <vector>

#include "../controller/controller.hpp"
#include "../include/grid.hpp"
#include "../include/main.h"
#include "gui_globals.h"

class SimpleCanvas {
   public:
    SimpleCanvas(SimpleCanvas& other) = delete;
    void operator=(const SimpleCanvas&) = delete;
    // Конструктор принимает GtkApplication
    static SimpleCanvas* GetInstance(GtkWidget* drawing_area);
    ~SimpleCanvas();
    void onDraw(cairo_t* cr);
    void redraw();
    void cleanup();
    // Статические колбэки GTK
    void init_gl(GtkGLArea *area);
    void draw(GdkGLContext *ctx);
    // Примитивы рисования
    void draw_line(cairo_t* cr, s21::Point start, s21::Point end);
    void draw_polygon(cairo_t* cr, const std::vector<s21::Point>& points);
    void draw_dot(cairo_t* cr, float x, float y);

    // Отрисовка фигуры
    void setDash(cairo_t* cr);
    void togglePolyFill();
    void setVertType(int type);
    void setLineType(int type);
    void setBgColor(Rgb color);
    void setVertColor(Rgb color);
    void setLineWidth(float width);
    void setVertSize(float size);
    void setProjection(s21::Poly_Proj_t proj);
    void updateFigure(std::vector<GLfloat> vertices, std::vector<GLuint> indices, std::vector<GLfloat> cube_vertices);
    void setMVP(const GLfloat* mvp);

   private:
    // singleton
    static std::unique_ptr<SimpleCanvas> simple_canvas_;
    explicit SimpleCanvas(GtkWidget* drawing_area);
    void setCanvas(cairo_t* cr);
    float scale_;
    int width_;
    int height_;
    float xStart_;
    float yStart_;
    s21::Poly_Proj_t projection_;
    GtkWidget* widget_;
    float lineWidth_;
    float vertWidth_;

    Rgb dotColor_;
    Rgb lineColor_;
    Rgb polyColor_;
    Rgb bgColor_;

    bool fillPoly_;
    Shape vertType_;
    Stroke lineType_;
    float canvas_scale_;

    GLuint shader_program;
    GLuint vao, vbo, ebo;
    GLuint mvp_location;
    GLuint poly_color_location;

    GLuint point_mvp_location;
    GLuint point_color_location;
    GLuint point_vao, point_vbo;
    GLuint point_program;

    GLuint texture;
    GLfloat mvp_[16];

    int vertCount;
    int polyIndicesCount;
};

#endif  // SIMPLE_CANVAS_HPP
