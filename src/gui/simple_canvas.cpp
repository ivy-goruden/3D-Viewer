#include "simple_canvas.hpp"

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <format>
#include <iostream>
#include "../include/axes.hpp"
#include "../include/globals.h"

static void on_realize(GtkGLArea *area, gpointer data) {
    SimpleCanvas* self = static_cast<SimpleCanvas*>(data);
    self->init_gl(area);
    std::cout << "realize" << std::endl;
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << err << std::endl;
}

// Вызывается при уничтожении GtkGLArea (освобождаем ресурсы)
static void on_unrealize(GtkGLArea *area, gpointer data) {
    gtk_gl_area_make_current(area);              // Делаем контекст текущим, чтобы удалять объекты
    if (!gtk_gl_area_get_error(area)) {          // Если контекст не повреждён
        SimpleCanvas* self = static_cast<SimpleCanvas*>(data);
        self->cleanup();
    }
}

void SimpleCanvas::cleanup(){
    glDeleteProgram(fong_program);
    glDeleteProgram(flatColor_program);
    glDeleteProgram(guro_program);
    glDeleteProgram(point_program);
    glDeleteVertexArrays(1, &vao);           // Удаляем VAO куба
    glDeleteBuffers(1, &vbo);                // Удаляем VBO куба
    glDeleteBuffers(1, &ebo);                // Удаляем EBO куба
    glDeleteVertexArrays(1, &point_vao);     // Удаляем VAO точек
    glDeleteBuffers(1, &point_vbo);     
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

SimpleCanvas::SimpleCanvas(GtkWidget* drawing_area)
    : scale_(1),
      width_(1),
      height_(1),
      xStart_(0),
      yStart_(0),
      projection_(),
      widget_(drawing_area),
      lineWidth_(1),
      vertWidth_(1),
      dotColor_(1, 0, 0, 1),
      lineColor_(0, 0, 0, 1),
      polyColor_(0, 0, 1, 1),
      bgColor_(1, 1, 1, 1),
      fillPoly_(false),
      vertType_(None),
      lineType_(Solid),
      canvas_scale_(1.0f),
      guro_program(0),
      flatColor_program(0),
      fong_program(0),
      vao(0), vbo(0), ebo(0),
      mvp_location(0), poly_color_location(0),
      point_vao(0), point_vbo(0), point_program(0),
      point_mvp_location(0), point_color_location(0),
      texture(0),
      vertCount(0),
      polyIndicesCount(0)
{
    gtk_gl_area_set_required_version(GTK_GL_AREA(drawing_area), 3, 3); // Требуем OpenGL 3.3
    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(drawing_area), TRUE); // Включаем буфер глубины
    gtk_gl_area_set_auto_render(GTK_GL_AREA(drawing_area), FALSE); // Мы будем сами запрашивать перерисовку
    g_signal_connect(drawing_area, "realize", G_CALLBACK(on_realize), this);   // При создании контекста
    g_signal_connect(drawing_area, "unrealize", G_CALLBACK(on_unrealize), this); // При удалении
    g_signal_connect(drawing_area, "render", G_CALLBACK(on_render), this);
    //g_timeout_add(16, on_timer, NULL);
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
    if (texture != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    glClearColor(bgColor_.red, bgColor_.green, bgColor_.blue, bgColor_.alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем экран и буфер глубины
    glUseProgram(getLightProgram());                // Активируем шейдерную программу
    glUniform3f(poly_color_location, polyColor_.red, polyColor_.green, polyColor_.blue);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp_); // Передаём матрицу MVP в uniform
    glBindVertexArray(vao);                      // Привязываем VAO
    glDisable(GL_DEPTH_TEST);
    // if (fillPoly_ == false){
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // }
    glDrawElements(GL_TRIANGLES, polyIndicesCount, GL_UNSIGNED_INT, 0); // Рисуем куб: 36 индексов, треугольники
    

    glUseProgram(getPointProgram());                 // Активируем шейдерную программу для точек
    glUniformMatrix4fv(point_mvp_location, 1, GL_FALSE, mvp_); // Передаём ту же матрицу MVP
    glUniform3f(point_color_location, dotColor_.red, dotColor_.green, dotColor_.blue);

    glPointSize(vertWidth_ / canvas_scale_);
    glDisable(GL_DEPTH_TEST);                    // Временно отключаем тест глубины – точки будут видны даже если перекрыты гранями
    glBindVertexArray(point_vao);                // Привязываем VAO точек
    glDrawArrays(GL_POINTS, 0, vertCount); 
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
void SimpleCanvas::setVertColor(Rgb color) { 
    dotColor_ = color; 
}

void SimpleCanvas::setLineWidth(float width) { lineWidth_ = width; }

void SimpleCanvas::setVertSize(float size) { vertWidth_ = size; }

void SimpleCanvas::setProjection(s21::Poly_Proj_t proj) { projection_ = proj; }


void SimpleCanvas::setMVP(const GLfloat* mvp){
    gtk_gl_area_make_current(GTK_GL_AREA(widget_));
    std::copy(mvp, mvp + 16, mvp_);
}

void SimpleCanvas::redraw() {
    if (widget_ != nullptr) {
        gtk_gl_area_queue_render(GTK_GL_AREA(widget_));
        std::cout << "redrew" << std::endl;
    }
}

void printVertices(const std::vector<GLfloat>& vertices) {
    std::cout << "Vertices (x, y, z, u, v):\n";
    for (size_t i = 0; i < vertices.size(); i += 5) {
        std::cout << "v" << i/5 << ": ("
                  << vertices[i] << ", " << vertices[i+1] << ", " << vertices[i+2]
                  << ") uv: (" << vertices[i+3] << ", " << vertices[i+4] << ")\n";
    }
}

void printIndices(const std::vector<GLuint>& indices) {
    std::cout << "Indices (triangles):\n";
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::cout << "tri " << i/3 << ": " 
                  << indices[i] << ", " << indices[i+1] << ", " << indices[i+2] << "\n";
    }
}

void printCubeVertices(const std::vector<GLfloat>& cube_vertices) {
    std::cout << "Cube vertices (only positions):\n";
    for (size_t i = 0; i < cube_vertices.size(); i += 3) {
        std::cout << "p" << i/3 << ": ("
                  << cube_vertices[i] << ", " << cube_vertices[i+1] << ", " << cube_vertices[i+2] << ")\n";
    }
}

void printMVP(const GLfloat* mvp, const char* name = "MVP") {
    std::cout << name << " matrix (column-major):\n";
    for (int row = 0; row < 4; ++row) {
        std::cout << "| ";
        for (int col = 0; col < 4; ++col) {
            std::cout << mvp[col * 4 + row] << "\t";
        }
        std::cout << "|\n";
    }
}

void SimpleCanvas::updateFigure(std::vector<GLfloat> vertices, std::vector<GLuint> indices, std::vector<GLfloat> cube_vertices){
                std::cout << "update figure" << std::endl;
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << err << std::endl;
    gtk_gl_area_make_current(GTK_GL_AREA(widget_));
    if (gtk_gl_area_get_error(GTK_GL_AREA(widget_))) return;
    glBindVertexArray(vao);                        // активируем VAO куба
    glBindBuffer(GL_ARRAY_BUFFER, vbo);            // привязываем VBO куба
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(GLfloat), // правильный размер
                 vertices.data(),
                 GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);    // привязываем EBO куба
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),   // правильный размер
                 indices.data(),
                 GL_STATIC_DRAW);
    
    glBindVertexArray(point_vao);                  // активируем VAO точек
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);      // привязываем point_vbo
    glBufferData(GL_ARRAY_BUFFER,
                 cube_vertices.size() * sizeof(GLfloat),            // если cube_vertices – массив фиксированного размера
                 cube_vertices.data(),GL_STATIC_DRAW);
    
    glBindVertexArray(0);                          // отвязываем VAO
    vertCount = vertices.size()/5;
    polyIndicesCount = indices.size();
    printVertices(vertices);
    // printIndices(indices);
    // printCubeVertices(cube_vertices);
    // printMVP(mvp_);
}

Light_Program SimpleCanvas::getLightProgram(){
    return fong_program;
}

Point_Program SimpleCanvas::getPointProgram(){
    return point_program;
}
