#include "simple_canvas.hpp"

#include <format>
#include <iostream>
#include <epoxy/gl.h>
#include "../include/axes.hpp"
#include "../include/globals.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"




static const char *fong_vertex =
    "#version 330 core\n"
    "\n"
    "// Входные данные от VBO\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "\n"
    "// Uniform-переменные (матрицы)\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "// Данные для фрагментного шейдера\n"
    "out vec3 FragPos;   // Позиция фрагмента в мировом пространстве\n"
    "out vec3 Normal;    // Нормаль фрагмента\n"
    "out vec2 TexCoord;  // Текстурные координаты\n"
    "\n"
    "void main() {\n"
    "    // Вычисляем позицию вершины в мировом пространстве\n"
    "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "    // Передаем нормаль (пока без учета масштабирования)\n"
    "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "    TexCoord = aTexCoord;\n"
    "\n"
    "    // Итоговая позиция в clip-space\n"
    "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\n";

static const char *fong_fragment = 
    "#version 330 core\n"
    "\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "in vec2 TexCoord;\n"
    "\n"
    "uniform vec3 lightPos;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 viewPos;\n"
    "uniform float ambientLight;\n"
    "\n"
    "uniform vec3 objectColor;\n"
    "uniform sampler2D ourTexture;\n"
    "\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main() {\n"
    "    vec4 texColor = texture(ourTexture, TexCoord);\n"
    "    vec3 color = texColor.rgb * objectColor;\n"
    "    vec3 ambient = ambientLight * lightColor;\n"
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 lightDir = normalize(lightPos - FragPos);\n"
    "    float diff = max(dot(norm, lightDir), 0.0);\n"
    "    vec3 diffuse = diff * lightColor;\n"
    "    float specularStrength = 0.5;\n"
    "    vec3 viewDir = normalize(viewPos - FragPos);\n"
    "    vec3 reflectDir = reflect(-lightDir, norm);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "    vec3 specular = specularStrength * spec * lightColor;\n"
    "    vec3 result = (ambient + diffuse + specular) * color;\n"
    "    FragColor = vec4(result, 1.0);\n"
    "}\n";

static const char *guro_vertex =
    "#version 330 core\n"
    "\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform vec3 lightPos;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 viewPos;\n"
    "uniform float ambientLight;\n"
    "uniform vec3 objectColor;\n"
    "\n"
    "out vec3 FragColor;\n"
    "out vec2 TexCoord;\n"
    "\n"
    "void main() {\n"
    "    vec3 worldPos = vec3(model * vec4(aPos, 1.0));\n"
    "    vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
    "    vec3 lightDir = normalize(lightPos - worldPos);\n"
    "    vec3 viewDir = normalize(viewPos - worldPos);\n"
    "    vec3 ambient = ambientLight * lightColor;\n"
    "    float diff = max(dot(normal, lightDir), 0.0);\n"
    "    vec3 diffuse = diff * lightColor;\n"
    "    float specularStrength = 0.5;\n"
    "    vec3 reflectDir = reflect(-lightDir, normal);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "    vec3 specular = specularStrength * spec * lightColor;\n"
    "    vec3 result = (ambient + diffuse + specular) * objectColor;\n"
    "    FragColor = result;\n"
    "    TexCoord = aTexCoord;\n"
    "    gl_Position = projection * view * vec4(worldPos, 1.0);\n"
    "}\n";

static const char *guro_fragment =
    "#version 330 core\n"
    "\n"
    "in vec3 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "out vec4 FragColorOut;\n"
    "\n"
    "void main() {\n"
    "    vec3 texColor = texture(ourTexture, TexCoord).rgb;\n"
    "    vec3 finalColor = FragColor * texColor;\n"
    "    FragColorOut = vec4(finalColor, 1.0);\n"
    "}\n";

static const char *flat_vertex =
    "#version 330 core\n"
    "\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "out vec3 FragPos;\n"
    "flat out vec3 Normal;\n"
    "out vec2 TexCoord;\n"
    "\n"
    "void main() {\n"
    "    vec4 worldPos = model * vec4(aPos, 1.0);\n"
    "    FragPos = worldPos.xyz;\n"
    "    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
    "    TexCoord = aTexCoord;\n"
    "    gl_Position = projection * view * worldPos;\n"
    "}\n";

static const char *flat_fragment =
    "#version 330 core\n"
    "\n"
    "in vec3 FragPos;\n"
    "flat in vec3 Normal;\n"
    "in vec2 TexCoord;\n"
    "\n"
    "uniform vec3 lightPos;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 viewPos;\n"
    "uniform float ambientLight;\n"
    "uniform vec3 objectColor;\n"
    "uniform sampler2D ourTexture;\n"
    "\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main() {\n"
    "    vec4 texColor = texture(ourTexture, TexCoord);\n"
    "    vec3 color = texColor.rgb * objectColor;\n"
    "    vec3 ambient = ambientLight * lightColor;\n"
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 lightDir = normalize(lightPos - FragPos);\n"
    "    float diff = max(dot(norm, lightDir), 0.0);\n"
    "    vec3 diffuse = diff * lightColor;\n"
    "    float specularStrength = 0.5;\n"
    "    vec3 viewDir = normalize(viewPos - FragPos);\n"
    "    vec3 reflectDir = reflect(-lightDir, norm);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "    vec3 specular = specularStrength * spec * lightColor;\n"
    "    vec3 result = (ambient + diffuse + specular) * color;\n"
    "    FragColor = vec4(result, 1.0);\n"
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

    // Атрибут 0: позиция
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Атрибут 1: нормали
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Атрибут 2: текстурные координаты (2 float)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
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
    GLint vs = compile_shader(GL_VERTEX_SHADER, fong_vertex);
    GLint fs = compile_shader(GL_FRAGMENT_SHADER, fong_fragment);
    fong_program.program = link_program(vs, fs);
    fong_program.model = glGetUniformLocation(fong_program.program, "model");
    fong_program.view = glGetUniformLocation(fong_program.program, "view");
    fong_program.projection = glGetUniformLocation(fong_program.program, "projection");
    fong_program.lightPos = glGetUniformLocation(fong_program.program, "lightPos");
    fong_program.lightColor = glGetUniformLocation(fong_program.program, "lightColor");
    fong_program.texture = glGetUniformLocation(fong_program.program, "ourTexture");
    fong_program.objectColor = glGetUniformLocation(fong_program.program, "objectColor");
    fong_program.ambientLight = glGetUniformLocation(fong_program.program, "ambientLight");
    fong_program.specularStrength = glGetUniformLocation(fong_program.program, "specularStrenght");
    // ---Гуро ---
    vs = compile_shader(GL_VERTEX_SHADER, guro_vertex);
    fs = compile_shader(GL_FRAGMENT_SHADER, guro_fragment);
    guro_program.program = link_program(vs, fs);
    guro_program.model = glGetUniformLocation(guro_program.program, "model");
    guro_program.view = glGetUniformLocation(guro_program.program, "view");
    guro_program.projection = glGetUniformLocation(guro_program.program, "projection");
    guro_program.lightPos = glGetUniformLocation(guro_program.program, "lightPos");
    guro_program.lightColor = glGetUniformLocation(guro_program.program, "lightColor");
    guro_program.texture = glGetUniformLocation(guro_program.program, "ourTexture");
    guro_program.objectColor = glGetUniformLocation(guro_program.program, "objectColor");
    guro_program.ambientLight = glGetUniformLocation(guro_program.program, "ambientLight");
    guro_program.specularStrength = glGetUniformLocation(guro_program.program, "specularStrenght");
    // ---Плоское ---
    vs = compile_shader(GL_VERTEX_SHADER, flat_vertex);
    fs = compile_shader(GL_FRAGMENT_SHADER, flat_fragment);
    flat_program.program = link_program(vs, fs);
    flat_program.model = glGetUniformLocation(flat_program.program, "model");
    flat_program.view = glGetUniformLocation(flat_program.program, "view");
    flat_program.projection = glGetUniformLocation(flat_program.program, "projection");
    flat_program.lightPos = glGetUniformLocation(flat_program.program, "lightPos");
    flat_program.lightColor = glGetUniformLocation(flat_program.program, "lightColor");
    flat_program.texture = glGetUniformLocation(flat_program.program, "ourTexture");
    flat_program.objectColor = glGetUniformLocation(flat_program.program, "objectColor");
    flat_program.ambientLight = glGetUniformLocation(flat_program.program, "ambientLight");
    flat_program.specularStrength = glGetUniformLocation(flat_program.program, "specularStrenght");
    // --- Создание программы для точек  ---
    vs = compile_shader(GL_VERTEX_SHADER, point_vert_src);
    fs = compile_shader(GL_FRAGMENT_SHADER, point_frag_src);
    point_program.program = link_program(vs, fs);
    point_program.mvp = glGetUniformLocation(point_program.program, "mvp");
    point_program.color = glGetUniformLocation(point_program.program, "pointColor");

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

