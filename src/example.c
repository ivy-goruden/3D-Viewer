#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <math.h>
#include <stdio.h>

// Простой треугольник: позиция (3 float) + цвет (3 float)
static const GLfloat vertices[] = {
    // x, y, z,    r,  g,  b
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // левый низ  – красный
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // правый низ – зелёный
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // верх       – синий
};

static GLuint shader_program;
static GLuint vao, vbo;
static GLuint mvp_location;

// Шейдеры – те же, что и в программе с кубом
static const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "uniform mat4 mvp;\n"
    "void main() {\n"
    "    gl_Position = mvp * vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "}\n";

static const char *fragment_shader_src =
    "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(ourColor, 1.0);\n"
    "}\n";

// Компиляция шейдера (без изменений)
static GLuint compile_shader(GLenum type, const char *src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        g_printerr("Shader error: %s\n", log);
        return 0;
    }
    return shader;
}

// Линковка программы (без изменений)
static GLuint link_program(GLuint vs, GLuint fs) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(prog, sizeof(log), NULL, log);
        g_printerr("Link error: %s\n", log);
        return 0;
    }
    return prog;
}

// Умножение двух column-major матриц 4x4: out = A * B
static void mat4_mult(const GLfloat *A, const GLfloat *B, GLfloat *out) {
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 4; row++)
            out[col * 4 + row] =
                A[0*4+row] * B[col*4+0] +
                A[1*4+row] * B[col*4+1] +
                A[2*4+row] * B[col*4+2] +
                A[3*4+row] * B[col*4+3];
}

static void init_gl(GtkGLArea *area) {
    gtk_gl_area_make_current(area);
    if (gtk_gl_area_get_error(area)) return;

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);
    shader_program = link_program(vs, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    mvp_location = glGetUniformLocation(shader_program, "mvp");

    // VAO и VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Атрибут позиции (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // Атрибут цвета   (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST); // для порядка, хотя треугольник один
}

// Вызывается при первой отрисовке и по явному запросу
static gboolean on_render(GtkGLArea *area, GdkGLContext *ctx, gpointer data) {
    gtk_gl_area_make_current(area);
    if (gtk_gl_area_get_error(area)) return FALSE;

    int w = gtk_widget_get_width(GTK_WIDGET(area));
    int h = gtk_widget_get_height(GTK_WIDGET(area));
    if (w == 0 || h == 0) return TRUE;
    float aspect = (float)w / (float)h;

    // === Заранее заданные матрицы (column-major) ===

    // 1. Model – единичная, треугольник остаётся на месте
    GLfloat model[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    // 2. View – камера в точке (0,0,3), смотрит в начало координат
    GLfloat view[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,-3,1
    };

    // 3. Projection – перспективная проекция (fov=60°, near=0.1, far=100)
    float fov = 60.0f * M_PI / 180.0f;
    float f = 1.0f / tanf(fov / 2.0f);
    float near = 0.1f, far = 100.0f;
    GLfloat projection[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, (2 * far * near) / (near - far), 0
    };

    // MVP = projection * view * model
    GLfloat mv[16], mvp[16];
    mat4_mult(view, model, mv);
    mat4_mult(projection, mv, mvp);

    glUseProgram(shader_program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);   // рисуем 3 вершины

    return TRUE;
}

// GTK сигналы
static void on_realize(GtkGLArea *area, gpointer data) {
    init_gl(area);
    // Отрисовываем один раз после инициализации
    gtk_gl_area_queue_render(area);
}

static void on_unrealize(GtkGLArea *area, gpointer data) {
    gtk_gl_area_make_current(area);
    if (!gtk_gl_area_get_error(area)) {
        glDeleteProgram(shader_program);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
}

static void activate(GtkApplication *app, gpointer data) {
    GtkWidget *win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win), "Static triangle with projection");
    gtk_window_set_default_size(GTK_WINDOW(win), 640, 480);

    GtkWidget *gl = gtk_gl_area_new();
    gtk_gl_area_set_required_version(GTK_GL_AREA(gl), 3, 3);
    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(gl), TRUE);
    gtk_gl_area_set_auto_render(GTK_GL_AREA(gl), FALSE);  // ручное управление

    g_signal_connect(gl, "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(gl, "unrealize", G_CALLBACK(on_unrealize), NULL);
    g_signal_connect(gl, "render", G_CALLBACK(on_render), NULL);

    gtk_window_set_child(GTK_WINDOW(win), gl);
    gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.example.static", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}