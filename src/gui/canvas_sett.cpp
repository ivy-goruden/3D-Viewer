#include "simple_canvas.hpp"

#include <format>
#include <iostream>
#include <epoxy/gl.h>
#include "../include/axes.hpp"
#include "../include/globals.h"

static const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 texCoord;\n"
    "uniform vec3 polyColor;\n"
    "out vec3 ourColor;\n"
    "uniform mat4 mvp;\n"
    "void main() {\n"
    "    gl_Position = mvp * vec4(aPos, 1.0);\n"
    "    ourColor = polyColor;\n"
    "}\n";

static const char *fragment_shader_src =
    "#version 330 core\n"
    "uniform vec3 polyColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(polyColor, 1.0);\n"
    "}\n";

static const char *point_vert_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"       // Только позиция (атрибут 0)
    "uniform mat4 mvp;\n"                         // Та же матрица MVP, что и для куба
    "void main() {\n"
    "    gl_Position = mvp * vec4(aPos, 1.0);\n"  // Преобразуем координаты вершины
    "}\n";

static const char *point_frag_src =
    "#version 330 core\n"
    "uniform vec3 pointColor;\n"                  // Uniform-переменная для цвета точки
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(pointColor, 1.0);\n"    // Используем переданный цвет
    "}\n";

// Компиляция одного шейдера заданного типа (вершинный или фрагментный)
static GLuint compile_shader(GLenum type, const char *src) {
    GLuint shader = glCreateShader(type);        // Создаём пустой шейдер нужного типа
    glShaderSource(shader, 1, &src, NULL);       // Загружаем исходный код GLSL в шейдер
    glCompileShader(shader);                     // Компилируем его
    GLint success;                               // Переменная для проверки успешности
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Получаем статус компиляции
    if (!success) {                              // Если ошибка...
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log); // ...получаем текст ошибки
        g_printerr("Shader compilation error: %s\n", log);  // ...выводим его
        return 0;                                // Возвращаем 0 как признак ошибки
    }
    return shader;                               // Возвращаем готовый шейдер
}

// Линковка вершинного и фрагментного шейдера в готовую программу
static GLuint link_program(GLuint vs, GLuint fs) {
    GLuint prog = glCreateProgram();             // Создаём пустую программу
    glAttachShader(prog, vs);                    // Присоединяем вершинный шейдер
    glAttachShader(prog, fs);                    // Присоединяем фрагментный шейдер
    glLinkProgram(prog);                         // Линкуем (собираем) программу
    GLint success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success); // Проверяем статус линковки
    if (!success) {                              // Если ошибка...
        char log[512];
        glGetProgramInfoLog(prog, sizeof(log), NULL, log); // ...получаем текст ошибки
        g_printerr("Program linking error: %s\n", log);
        return 0;
    }
    return prog;                                 // Возвращаем готовую программу
}

void SimpleCanvas::init_gl(GtkGLArea *area) {
    gtk_gl_area_make_current(area);              // Делаем OpenGL-контекст текущим для этого виджета
    if (gtk_gl_area_get_error(area)) return;     // Если возникла ошибка, выходим

    // --- Создание программы для куба ---
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);   // Компилируем вершинный шейдер куба
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src); // Компилируем фрагментный шейдер куба
    shader_program = link_program(vs, fs);        // Линкуем их в программу
    glDeleteShader(vs);                           // Шейдеры больше не нужны, удаляем
    glDeleteShader(fs);
    mvp_location = glGetUniformLocation(shader_program, "mvp"); // Получаем "адрес" uniform-переменной mvp

    // --- Создание программы для точек (вершин куба) ---
    GLuint pvs = compile_shader(GL_VERTEX_SHADER, point_vert_src); // Вершинный шейдер для точек
    GLuint pfs = compile_shader(GL_FRAGMENT_SHADER, point_frag_src); // Фрагментный шейдер для точек
    point_program = link_program(pvs, pfs);       // Собираем программу для точек
    glDeleteShader(pvs);                          // Удаляем шейдеры (они уже в программе)
    glDeleteShader(pfs);
    point_mvp_location = glGetUniformLocation(point_program, "mvp");     // Адрес uniform mvp в программе точек
    point_color_location = glGetUniformLocation(point_program, "pointColor"); // Адрес uniform pointColor
    poly_color_location = glGetUniformLocation(shader_program, "polyColor"); // Адрес uniform pointColor




    // --- Создание VAO, VBO и EBO для куба ---
    glGenVertexArrays(1, &vao);                  // Генерируем идентификатор VAO для куба
    glGenBuffers(1, &vbo);                       // Генерируем идентификатор VBO (вершинный буфер)
    glGenBuffers(1, &ebo);                       // Генерируем идентификатор EBO (индексный буфер)

    glBindVertexArray(vao);                      // Привязываем VAO – все следующие настройки относятся к нему

    glBindBuffer(GL_ARRAY_BUFFER, vbo);          // Привязываем VBO как массив вершин

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);  // Привязываем EBO как индексный буфер

    // Настройка атрибутов вершин куба (позиция и цвет)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Атрибут 1: текстурные координаты (2 float)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);                        // Отвязываем VAO (настройки запомнены)

    // --- Создание VAO и VBO для 8 вершин куба (точек) ---
    glGenVertexArrays(1, &point_vao);            // Новый VAO для точек
    glGenBuffers(1, &point_vbo);                 // VBO для вершин куба (только координаты)
    glBindVertexArray(point_vao);                // Привязываем VAO точек
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);    // Привязываем VBO
    // В этом VBO только позиция (3 float), атрибут 0, без сдвига и шага
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);                // Включаем атрибут 0 для точек
    glBindVertexArray(0);                        // Отвязываем VAO точек

    // --- Глобальные настройки OpenGL ---
    glEnable(GL_DEPTH_TEST);                     // Включаем тест глубины (чтобы ближние грани перекрывали дальние)
    glClearColor(bgColor_.red, bgColor_.green, bgColor_.blue, bgColor_.alpha);
}