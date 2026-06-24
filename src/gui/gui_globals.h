#ifndef GUI_GLOBALS
#define GUI_GLOBALS

#include <iostream>
#include <gtk/gtk.h>
#include <epoxy/gl.h>

enum Shape { None, Circle, Rect };
enum Stroke { Solid, Dotted };
struct Rgb {
    float red;
    float green;
    float blue;
    float alpha;

    Rgb() : red(0), green(0), blue(0), alpha(0) {}

    Rgb(float r, float g, float b, float a = 1) : red(r), green(g), blue(b), alpha(a) {}

    Rgb operator*(Rgb& other) const { return Rgb(red * other.red, green * other.green, blue * other.blue); }

    Rgb operator*(float other) const { return Rgb(red * other, green * other, blue * other); }
};

inline void checkGLError(const char* file, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << err << " at " << file << ":" << line << std::endl;
    }
}
#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)
#endif