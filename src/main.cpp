#include "include/main.h"
#include "simple_canvas.hpp"
using namespace s21;

void activate_cb(GtkApplication* app, gpointer user_data) {
    new SimpleCanvas(app);
}

int main(int argc, char* argv[]){
    GtkApplication* app = gtk_application_new("com.example.canvas",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate_cb), nullptr);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}