#include "simple_canvas.hpp"

#include <format>
#include <iostream>
#include <epoxy/gl.h>
#include "../include/axes.hpp"
#include "../include/globals.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"


static const char *fong_vertex =
    "#version 330 core

    // Входные данные от VBO
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;

    // Uniform-переменные (матрицы)
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    // Данные для фрагментного шейдера
    out vec3 FragPos;   // Позиция фрагмента в мировом пространстве
    out vec3 Normal;    // Нормаль фрагмента
    out vec2 TexCoord;  // Текстурные координаты

    void main() {
        // Вычисляем позицию вершины в мировом пространстве
        FragPos = vec3(model * vec4(aPos, 1.0));
        // Передаем нормаль (пока без учета масштабирования)
        Normal = mat3(transpose(inverse(model))) * aNormal;
        TexCoord = aTexCoord;

        // Итоговая позиция в clip-space
        gl_Position = projection * view * vec4(FragPos, 1.0);
    }\n"

static const char *fong_fragment = 
    "#version 330 core

    // Входные данные от вершинного шейдера
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;

    // Uniform-переменные
    uniform vec3 lightPos;   // Позиция источника света в мировом пространстве
    uniform vec3 lightColor; // Цвет источника света
    uniform vec3 viewPos;    // Позиция камеры в мировом пространстве
    uniform vec1 ambientLight; 

    // Свойства материала
    uniform vec3 objectColor;
    uniform sampler2D ourTexture;
    uniform 

    out vec4 FragColor;

    void main() {
        // Получаем цвет текстуры
        vec4 texColor = texture(ourTexture, TexCoord);
        vec3 color = texColor.rgb * objectColor;

        // --- Ambient (Фоновое освещение) ---
        vec3 ambient = ambientLight * lightColor;

        // --- Diffuse (Рассеянный свет) ---
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // --- Specular (Блики) ---
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        // Итоговый цвет
        vec3 result = (ambient + diffuse + specular) * color;
        FragColor = vec4(result, 1.0);
    }\n"

static const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 texCoord;\n"
    "uniform vec3 polyColor;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 mvp;\n"
    "void main() {\n"
    "    gl_Position = mvp * vec4(aPos, 1.0);"
    "    TexCoord = texCoord;"
    "}\n";

static const char *fragment_shader_src =
    "#version 330 core\n"
    "uniform vec3 polyColor;\n"
    "uniform sampler2D ourTexture;"
    "in vec2 TexCoord;"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    vec4 texColor = texture(ourTexture, TexCoord);"
    "    FragColor = vec4(texColor.rgb * polyColor, texColor.a);"
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
    initPrograms();
    initTexture();
    initFigure();
    initDots();

    glEnable(GL_DEPTH_TEST);
    glClearColor(bgColor_.red, bgColor_.green, bgColor_.blue, bgColor_.alpha);
}

inline void SimpleCanvas::initFigure(){
    // --- Создание VAO, VBO и EBO ---
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
}

inline void SimpleCanvas::initDots(){
    // --- Создание VAO и VBO для точек) ---
    glGenVertexArrays(1, &point_vao);            // Новый VAO для точек
    glGenBuffers(1, &point_vbo);                 // VBO для вершин куба (только координаты)
    glBindVertexArray(point_vao);                // Привязываем VAO точек
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);    // Привязываем VBO
    // В этом VBO только позиция (3 float), атрибут 0, без сдвига и шага
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);                // Включаем атрибут 0 для точек
    glBindVertexArray(0);                        // Отвязываем VAO точек
}

inline void SimpleCanvas::initPrograms(){

    // ---Фонг ---
    vs = compile_shader(GL_VERTEX_SHADER, fong_vertex);
    fs = compile_shader(GL_FRAGMENT_SHADER, fong_fragment);
    fong_program.program = link_program(vs, fs);
    fong_program.model = glGetUniformLocation(fong_program, "model");
    fong_program.view = glGetUniformLocation(fong_program, "view");
    fong_program.projection = glGetUniformLocation(fong_program, "projection");
    fong_program.lightPos = glGetUniformLocation(fong_program, "lightPos");
    fong_program.lightColor = glGetUniformLocation(fong_program, "lightColor");
    fong_program.texture = glGetUniformLocation(fong_program, "ourTexture");
    fong_program.objectColor = glGetUniformLocation(fong_program, "objectColor");
    fong_program.ambientLight = glGetUniformLocation(fong_program, "ambientLight")
    fong_program.specularStrength = glGetUniformLocation(fong_program, "specularStrenght");
    // --- Создание программы для точек  ---
    vs = compile_shader(GL_VERTEX_SHADER, point_vert_src);
    fs = compile_shader(GL_FRAGMENT_SHADER, point_frag_src);
    point_program.program = link_program(vs, fs);
    point_program.mvp = glGetUniformLocation(point_program, "mvp");
    point_program.color = glGetUniformLocation(point_program, "pointColor");

    glDeleteShader(vs);
    glDeleteShader(fs);
}

inline void SimpleCanvas::initTexture(){
    // Загрузка текстуры
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Настройка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    loadTexture("/home/enolagal/3D-Viewer/texture.jpg");
    //loadTexture("/home/enolagal/3D-Viewer/images.jpeg");
    glUseProgram(shader_program);

    GLint texLoc = glGetUniformLocation(shader_program, "ourTexture");
    // Устанавливаем, что текстура будет браться из текстурного юнита 0
    if (texLoc != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);   // привязываем нашу текстуру к юниту 0
        glUniform1i(texLoc, 0);
    } else {
        std::cerr << "Uniform 'ourTexture' not found in shader!" << std::endl;
    }
    CHECK_GL_ERROR();
}

void SimpleCanvas::loadTexture(const char* file){
    // Загрузка изображения через stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Не удалось загрузить текстуру" << std::endl;
    }
    stbi_image_free(data);
}

