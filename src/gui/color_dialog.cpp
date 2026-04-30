#include "color_dialog.hpp"

#include <gtk/gtk.h>
#include <string>
#include "app_data.hpp"

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

void ColorDialog::open(Rgb color) {
    g_print("Нач цвет: r=%.2f g=%.2f b=%.2f a=%.2f\n", color.red, color.green, color.blue, color.alpha);

    GdkRGBA initial_color = {
        .red = float(color.red),
        .green = float(color.green),
        .blue = float(color.blue),
        .alpha = float(color.alpha)
    };

    active = true;
    GtkColorDialog *dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(dialog, "Выберите цвет");
    gtk_color_dialog_choose_rgba(dialog, GTK_WINDOW(window), &initial_color, NULL, onColorSelected, this);
    active = false;
}

bool ColorDialog::isActive() {
    return active;
}
