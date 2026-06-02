#ifndef GUI_GLOBALS
#define GUI_GLOBALS

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
#endif