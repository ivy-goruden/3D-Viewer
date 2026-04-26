#include "include/main.h"
#include "simple_canvas.hpp"
#include "callbacks/callbacks.hpp"
#include "config/config.hpp"
#include <valgrind/valgrind.h>
using namespace s21;

int main(int argc, char* argv[]){
    VALGRIND_DISCARD_TRANSIENT_ALLOCATIONS();
    GtkApplication* app = gtk_application_new("com.example.canvas",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(Config::config_GTK), nullptr);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}