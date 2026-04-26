#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
#include <memory>
#include "../simple_canvas.hpp"

namespace s21{

    class Callbacks {
      public:
        static void on_load_button_clicked(GtkWidget *button, gpointer user_data);
        static void on_down_scale_clicked(GtkWidget *button, gpointer user_data);
        static void on_up_scale_clicked(GtkWidget *button, gpointer user_data);
        static void on_AbsRotate_clicked(GtkWidget *button, gpointer user_data);
        static void on_rotateX_clicked(GtkWidget *button, gpointer user_data);
        static void on_rotateY_clicked(GtkWidget *button, gpointer user_data);
        static void on_perspectiveOn_clicked(GtkWidget *button, gpointer user_data);
        static void print_hello(GtkWidget *widget, gpointer data);
    };

}

#endif // CALLBACKS_H
