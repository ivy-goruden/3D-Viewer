#include <gtk/gtk.h>
 
void openButtonClick(GtkButton* btn) {
    g_print("open button\n");
}

void saveButtonClick(GtkButton* btn) {
    g_print("save button\n");
}

void resetButtonClick(GtkButton* btn) {
    g_print("reset button\n");
}

void xSpinnerValueChanged(GtkSpinButton* self, gpointer user_data) {
    double value = gtk_spin_button_get_value(self);
    g_print("%s: %.0f\n", "xSpinnerValueVhanged", value);
}

void ySpinnerValueChanged(GtkSpinButton* self, gpointer user_data) {
    double value = gtk_spin_button_get_value(self);
    g_print("%s: %.0f\n", "ySpinnerValueVhanged", value);
}

void zSpinnerValueChanged(GtkSpinButton* self, gpointer user_data) {
    double value = gtk_spin_button_get_value(self);
    g_print("%s: %.0f\n", "zSpinnerValueVhanged", value);
}

void shiftSpinnerValueChanged(GtkSpinButton* self, gpointer user_data) {
    double value = gtk_spin_button_get_value(self);
    g_print("%s: %.0f\n", "shiftSpinnerValueVhanged", value);
}

void zoomSpinnerValueChanged(GtkSpinButton* self, gpointer user_data) {
    double value = gtk_spin_button_get_value(self);
    g_print("%s: %.0f\n", "zoomSpinnerValueVhanged", value);
}

void app_activate (GtkApplication *app, gpointer *user_data) {
    GtkBuilder* builder = gtk_builder_new_from_file("gui/3dviewer.ui");
    GObject* window = gtk_builder_get_object(builder, "main");
    GObject* openButton = gtk_builder_get_object(builder, "open_button");
    GObject* saveButton = gtk_builder_get_object(builder, "save_button");
    GObject* resetButton = gtk_builder_get_object(builder, "reset_button");
    GObject* xSpinnerButton = gtk_builder_get_object(builder, "rotate_x_spinner");
    GObject* ySpinnerButton = gtk_builder_get_object(builder, "rotate_y_spinner");
    GObject* zSpinnerButton = gtk_builder_get_object(builder, "rotate_z_spinner");
    GObject* shiftSpinnerButton = gtk_builder_get_object(builder, "shift_spinner");
    GObject* zoomSpinnerButton = gtk_builder_get_object(builder, "zoom_spinner");
    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClick), NULL);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClick), NULL);
    g_signal_connect(resetButton, "clicked", G_CALLBACK(resetButtonClick), NULL);
    g_signal_connect (xSpinnerButton, "value_changed", G_CALLBACK (xSpinnerValueChanged), NULL);
    g_signal_connect (ySpinnerButton, "value_changed", G_CALLBACK (ySpinnerValueChanged), NULL);
    g_signal_connect (zSpinnerButton, "value_changed", G_CALLBACK (zSpinnerValueChanged), NULL);
    g_signal_connect (shiftSpinnerButton, "value_changed", G_CALLBACK (shiftSpinnerValueChanged), NULL);
    g_signal_connect (zoomSpinnerButton, "value_changed", G_CALLBACK (zoomSpinnerValueChanged), NULL);
    g_object_unref(builder);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));
}
  
int main (int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.metanit", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (app_activate), NULL);
    int status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);
    return status;
}
