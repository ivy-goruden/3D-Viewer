#include "color_dialog.hpp"

#include <gtk/gtk.h>
#include <string>

void ColorDialog::setOnColorSelectedCallback(ColorSelectedCallback callback) {
    onColorSelectedCallback = callback;
}

void ColorDialog::onColorSelected(GObject *source, GAsyncResult *result, gpointer user_data) {
    ColorDialog *self = static_cast<ColorDialog*>(user_data);    
    GtkColorDialog *dialog = GTK_COLOR_DIALOG(source);
    GError *error = NULL;    
    GdkRGBA *color = gtk_color_dialog_choose_rgba_finish(dialog, result, &error);
    if (color) {
        if (self->onColorSelectedCallback) {
            self->onColorSelectedCallback(color->red, color->green, color->blue, color->alpha);
        }
        g_free(color);
    } else if (error) {
        if (!g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_CANCELLED)) {
            g_printerr("Ошибка: %s\n", error->message);
        }
        g_error_free(error);        
    }
}

void ColorDialog::open() {
    active = true;
    GtkColorDialog *dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(dialog, "Выберите цвет");
    gtk_color_dialog_choose_rgba(dialog, GTK_WINDOW(window), NULL, NULL, onColorSelected, this);
    active = false;
}

bool ColorDialog::isActive() {
    return active;
}
