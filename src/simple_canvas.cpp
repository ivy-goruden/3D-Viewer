#include "simple_canvas.hpp"

SimpleCanvas::SimpleCanvas(){
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Algorithm Testing Canvas");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
    
    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_static), this);
    
    gtk_widget_show_all(window);
}

// void SimpleCanvas::draw_line(cairo_t* cr, double x1, double y1, double x2, double y2){
//     cairo_set_source_rgb(cr, 0, 1, 0); // Green
//     cairo_set_line_width(cr, 2);
//     cairo_move_to(cr, x1, y1);
//     cairo_line_to(cr, x2, y2);
//     cairo_stroke(cr);
// }

void SimpleCanvas::draw_polygon(cairo_t* cr, const std::vector<Point>& points) {
    if (points.size() < 3) return;
    
    cairo_set_source_rgb(cr, 1, 0, 0); // Red
    cairo_set_line_width(cr, 2);
    
    cairo_move_to(cr, points[0].x, points[0].y);
    for (size_t i = 1; i < points.size(); i++) {
        cairo_line_to(cr, points[i].x, points[i].y);
    }
    cairo_close_path(cr);
    cairo_stroke(cr);
}

void SimpleCanvas::draw_dot(cairo_t* cr, double x, double y) {
    cairo_arc(cr, x, y, 5, 0, 2 * M_PI);
    cairo_set_source_rgb(cr, 0, 0, 1); // Blue
    cairo_fill(cr);
}

void SimpleCanvas::my_algorithm(cairo_t* cr) {
        // Example: Draw some shapes to test your algorithm
        
        // Draw dots at specific positions
        draw_dot(cr, 100, 100);
        draw_dot(cr, 200, 150);
        draw_dot(cr, 300, 200);
        
        // Draw lines between points
        draw_line(cr, 100, 100, 400, 100);
        draw_line(cr, 400, 100, 400, 300);
        
        // Draw polygon (several dots connected)
        std::vector<Point> polygon;
        polygon.push_back(Point(500, 100));
        polygon.push_back(Point(600, 150));
        polygon.push_back(Point(550, 250));
        polygon.push_back(Point(450, 200));
        draw_polygon(cr, polygon);
        
        // You can add more test shapes here
        // draw_dot(cr, x, y);
        // draw_line(cr, x1, y1, x2, y2);
        // draw_polygon(cr, vector_of_points);
}
gboolean on_draw_static(GtkWidget* widget, cairo_t* cr, gpointer data) {
        SimpleCanvas* canvas = static_cast<SimpleCanvas*>(data);
        
        // White background
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_paint(cr);
        
        // Run your algorithm here
        canvas->my_algorithm(cr);
        
        return FALSE;
}
