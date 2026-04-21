#include "callbacks.hpp"

namespace s21{
void Callbacks::on_load_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget* window = GTK_WIDGET(user_data);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(G_OBJECT(window), "canvas"));
    if (canvas) {
        (*canvas)->loadFigure("./assets/cube.obj");
        (*canvas)->redraw();
    }
    
}

void Callbacks::on_down_scale_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget* window = GTK_WIDGET(user_data);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(G_OBJECT(window), "canvas"));
    (*canvas)->setScale(20.0);
    (*canvas)->redraw();
}

void Callbacks::on_up_scale_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget* window = GTK_WIDGET(user_data);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(G_OBJECT(window), "canvas"));
    (*canvas)->setScale(100.0);
    (*canvas)->redraw();
}

void Callbacks::on_rotateA_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget* window = GTK_WIDGET(user_data);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(G_OBJECT(window), "canvas"));
    (*canvas)->rotate(135,90,0);
    (*canvas)->redraw();
}

void Callbacks::on_rotateB_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget* window = GTK_WIDGET(user_data);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(G_OBJECT(window), "canvas"));
    (*canvas)->rotate(0,0,0);
    (*canvas)->redraw();
}

void Callbacks::on_rotateC_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget* window = GTK_WIDGET(user_data);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(G_OBJECT(window), "canvas"));
    (*canvas)->rotate(0,0,45);
    (*canvas)->redraw();
}

void Callbacks::on_perspectiveOn_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget* window = GTK_WIDGET(user_data);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(G_OBJECT(window), "canvas"));
    (*canvas)->toggleProjection();
    (*canvas)->redraw();
}

void Callbacks::print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

}