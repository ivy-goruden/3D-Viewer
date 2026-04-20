#include "include/main.h"
#include "simple_canvas.hpp"
#include <memory>
using namespace s21;

static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

// Обработчик закрытия окна
void on_window_destroy(GtkWidget* widget, gpointer data) {
    GApplication* app = g_application_get_default();
    if (app) {
        g_application_quit(app);
    }
}

static void on_load_button_clicked(GtkWidget *button, gpointer user_data) {
    auto* canvas_ptr = static_cast<std::shared_ptr<SimpleCanvas>*>(user_data);
    (*canvas_ptr)->loadFigure("./assets/cube.obj");
    (*canvas_ptr)->redraw();
}

static void on_down_scale_clicked(GtkWidget *button, gpointer user_data) {
    auto* canvas_ptr = static_cast<std::shared_ptr<SimpleCanvas>*>(user_data);
    (*canvas_ptr)->setScale(20.0);
    (*canvas_ptr)->redraw();
}

static void on_up_scale_clicked(GtkWidget *button, gpointer user_data) {
    auto* canvas_ptr = static_cast<std::shared_ptr<SimpleCanvas>*>(user_data);
    (*canvas_ptr)->setScale(100.0);
    (*canvas_ptr)->redraw();
}

static void on_rotateA_clicked(GtkWidget *button, gpointer user_data) {
    auto* canvas_ptr = static_cast<std::shared_ptr<SimpleCanvas>*>(user_data);
    (*canvas_ptr)->rotate(135,90,0);
    (*canvas_ptr)->redraw();
}

static void on_rotateB_clicked(GtkWidget *button, gpointer user_data) {
    auto* canvas_ptr = static_cast<std::shared_ptr<SimpleCanvas>*>(user_data);
    (*canvas_ptr)->rotate(0,0,0);
    (*canvas_ptr)->redraw();
}

void addControls(GtkWidget* box, std::shared_ptr<SimpleCanvas>* canvas){
    // Create the button
    GtkWidget *button = gtk_button_new_with_label("Hello World");
    GtkWidget *load_btn = gtk_button_new_with_label("Load OBJ");
    GtkWidget *upscale = gtk_button_new_with_label("+");
    GtkWidget *downscale = gtk_button_new_with_label("-");
    GtkWidget *rotateA = gtk_button_new_with_label("rotateA");
    GtkWidget *rotateB = gtk_button_new_with_label("rotateB");

    g_signal_connect(upscale, "clicked", G_CALLBACK(on_up_scale_clicked), canvas);
    g_signal_connect(downscale, "clicked", G_CALLBACK(on_down_scale_clicked), canvas);
    g_signal_connect(load_btn, "clicked", G_CALLBACK(on_load_button_clicked), canvas);
    g_signal_connect(rotateA, "clicked", G_CALLBACK(on_down_scale_clicked), canvas);
    g_signal_connect(rotateB, "clicked", G_CALLBACK(on_load_button_clicked), canvas);


    // Add button to box
    gtk_box_append(GTK_BOX(box), load_btn);
    gtk_box_append(GTK_BOX(box), upscale);
    gtk_box_append(GTK_BOX(box), downscale);
    gtk_box_append(GTK_BOX(box), rotateA);
    gtk_box_append(GTK_BOX(box), rotateB);
}

void activate_cb(GtkApplication* app, gpointer user_data) {
    
    GtkWidget* window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Algorithm Testing Canvas");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_application(GTK_WINDOW(window), app);

    // Create a container box
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Drawing area
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 600, 500);
    gtk_box_append(GTK_BOX(box), drawing_area);
    gtk_box_append(GTK_BOX(box), controls);

    // Создаём shared_ptr для объекта
    auto canvas = std::make_shared<SimpleCanvas>(drawing_area);
    
    // Сохраняем shared_ptr в куче, чтобы передать в колбэки
    auto* canvas_ptr = new std::shared_ptr<SimpleCanvas>(canvas);
    
    // Привязываем к окну, чтобы удалить shared_ptr при уничтожении окна
    g_object_set_data_full(G_OBJECT(window), "canvas_ptr", canvas_ptr,
                           [](gpointer data) {
                               delete static_cast<std::shared_ptr<SimpleCanvas>*>(data);
                           });

    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(drawing_area, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(controls, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), box);

    addControls(controls, canvas_ptr);

    gtk_box_append(GTK_BOX(box), drawing_area);
    gtk_window_present(GTK_WINDOW(window));
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), nullptr);
}



int main(int argc, char* argv[]){
    GtkApplication* app = gtk_application_new("com.example.canvas",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate_cb), nullptr);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}