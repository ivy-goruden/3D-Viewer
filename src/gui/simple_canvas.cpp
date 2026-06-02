#include "simple_canvas.hpp"

#include <format>
#include <iostream>
#include "../include/axes.hpp"
#include "../include/globals.h"

static void on_realize(GtkGLArea *area, gpointer data) {
    SimpleCanvas* self = static_cast<SimpleCanvas*>(data);
    self->init_gl(area);
}

// Вызывается при уничтожении GtkGLArea (освобождаем ресурсы)
static void on_unrealize(GtkGLArea *area, gpointer data) {
    gtk_gl_area_make_current(area);              // Делаем контекст текущим, чтобы удалять объекты
    if (!gtk_gl_area_get_error(area)) {          // Если контекст не повреждён
        SimpleCanvas* self = static_cast<SimpleCanvas*>(data);
        self->delete();
    }
}

// Таймер, который запрашивает перерисовку каждые 16 мс (~60 кадров в секунду)
static gboolean on_timer(gpointer data) {
    SimpleCanvas* self = static_cast<SimpleCanvas*>(data);
    self->redraw();
    return TRUE;                                  // Возвращаем TRUE, чтобы таймер продолжал срабатывать
}

// Основная функция рендеринга – вызывается каждый кадр
static gboolean on_render(GtkGLArea *area, GdkGLContext *ctx, gpointer data) {
    gtk_gl_area_make_current(area);              // Активируем OpenGL-контекст для рисования
    if (gtk_gl_area_get_error(area)) return FALSE; // Проверка ошибок
    SimpleCanvas* self = static_cast<SimpleCanvas*>(data);
    self->draw(ctx);
    return TRUE;
}

SimpleCanvas::SimpleCanvas(GtkWidget* drawing_area) : vao{}, vbo{}, ebo{}, point_vao{}, point_vbo{}, mvp_{}{
    gtk_gl_area_set_required_version(GTK_GL_AREA(drawing_area), 3, 3); // Требуем OpenGL 3.3
    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(drawing_area), TRUE); // Включаем буфер глубины
    gtk_gl_area_set_auto_render(GTK_GL_AREA(drawing_area), FALSE); // Мы будем сами запрашивать перерисовку
    g_signal_connect(drawing_area, "realize", G_CALLBACK(on_realize), NULL);   // При создании контекста
    g_signal_connect(drawing_area, "unrealize", G_CALLBACK(on_unrealize), NULL); // При удалении
    g_signal_connect(drawing_area, "render", G_CALLBACK(on_render), NULL);
    //g_timeout_add(16, on_timer, NULL);
    scale_ = 1;
    xStart_ = 0;
    yStart_ = 0;
    widget_ = drawing_area;
    dotColor_ = Rgb(1, 0, 0, 1);
    lineColor_ = Rgb(0, 0, 0, 1);
    polyColor_ = Rgb(0, 0, 1, 1);
    bgColor_ = Rgb(1, 1, 1, 1);
    fillPoly_ = false;
    lineWidth_ = 1;
    vertWidth_ = 1;
    vertType_ = None;
    lineType_ = Solid;
    width_ = 1;
    height_ = 1;
    s21::Poly_Proj_t projection_ = s21::Poly_Proj_t();

}

std::unique_ptr<SimpleCanvas> SimpleCanvas::simple_canvas_ = nullptr;

SimpleCanvas* SimpleCanvas::GetInstance(GtkWidget* drawing_area) {
    if (!simple_canvas_) {
        simple_canvas_ = std::unique_ptr<SimpleCanvas>(new SimpleCanvas(drawing_area));
    }
    return simple_canvas_.get();
}

SimpleCanvas::~SimpleCanvas() {}

void SimpleCanvas::draw_dot(cairo_t* cr, float x, float y) {
    switch (vertType_) {
        case Circle:
            cairo_arc(cr, x, y, vertWidth_ / canvas_scale_ * 2, 0, 2 * M_PI);
            break;
        case Rect:
            float d = vertWidth_ / canvas_scale_ * 2 * std::sqrt(2) / 2;
            cairo_rectangle(cr, x - d, y - d, 2 * d, 2 * d);
            break;
    }
    cairo_set_source_rgb(cr, dotColor_.red, dotColor_.green, dotColor_.blue);
    cairo_fill(cr);
}

void SimpleCanvas::draw(GdkGLContext *ctx){
    glUseProgram(shader_program);                // Активируем шейдерную программу куба
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp_); // Передаём матрицу MVP в uniform

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем экран и буфер глубины
    glBindVertexArray(vao);                      // Привязываем VAO куба (все настройки вершин)
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Рисуем куб: 36 индексов, треугольники

    // --- Рисуем 8 вершин куба точками ПОВЕРХ ---
    glUseProgram(point_program);                 // Активируем шейдерную программу для точек
    glUniformMatrix4fv(point_mvp_location, 1, GL_FALSE, mvp_); // Передаём ту же матрицу MVP
    // Задаём цвет точек (белый)

    glPointSize(vertWidth_ / canvas_scale_);
    glDisable(GL_DEPTH_TEST);                    // Временно отключаем тест глубины – точки будут видны даже если перекрыты гранями
    glBindVertexArray(point_vao);                // Привязываем VAO точек
    glDrawArrays(GL_POINTS, 0, 8);               // Рисуем 8 точек (по количеству вершин в буфере)
    glEnable(GL_DEPTH_TEST);                     // Включаем тест глубины обратно
    glBindVertexArray(0);                        // Отвязываем VAO
}
    

void SimpleCanvas::setCanvas(cairo_t* cr) {
    if (width_ <= 0 || height_ <= 0) return;
    cairo_translate(cr, width_ / 2.0, height_ / 2.0);
    cairo_scale(cr, 1, 1);
}

void SimpleCanvas::togglePolyFill() { fillPoly_ = !fillPoly_; }

void SimpleCanvas::setVertType(int type) {
    switch (type) {
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

void SimpleCanvas::setLineType(int type) {
    switch (type) {
        case 1:
            lineType_ = Stroke(Dotted);
            break;
        default:
            lineType_ = Stroke(Solid);
            break;
    }
}

void SimpleCanvas::setBgColor(Rgb color) { bgColor_ = color; }
void SimpleCanvas::setVertColor(Rgb color) { dotColor_ = color; glUniform3f(point_color_location, dotColor_.red, dotColor_.green, dotColor_.blue); }

void SimpleCanvas::setLineWidth(float width) { lineWidth_ = width; }

void SimpleCanvas::setVertSize(float size) { vertWidth_ = size; }

void SimpleCanvas::setProjection(s21::Poly_Proj_t proj) { projection_ = proj; }


void SimpleCanvas::setMVP(const GLfloat* mvp){
    std::copy(mvp, mvp + 16, mvp_);
}

void SimpleCanvas::redraw() {
    if (widget_ != nullptr) {
        gtk_gl_area_queue_render(GTK_GL_AREA(widget_));
    }
}

void SimpleCanvas::updateFigure(std::vector<GLfloat> vertices, std::vector<GLuint> indices){
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Загружаем вершины куба в буфер
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Загружаем индексы
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Загружаем 8 вершин
}
