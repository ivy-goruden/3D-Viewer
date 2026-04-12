#include <gtk/gtk.h>
#include <vector>
#include <cmath>

class SimpleCanvas {
private:
    GtkWidget* drawing_area;
    double scale_;
    double xStart_;
    double yStart_;
    void my_algorithm(cairo_t* cr);
    static gboolean on_draw_static(GtkWidget* widget, cairo_t* cr, gpointer data);
    
public:
    SimpleCanvas():
    
    void draw_dot(cairo_t* cr, double x, double y);
    
    //void draw_line(cairo_t* cr, double x1, double y1, double x2, double y2);
    
    void draw_polygon(cairo_t* cr, const std::vector<Point>& points);
    
};

