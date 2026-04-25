
// Config.h
#ifndef S21_CONFIG_H
#define S21_CONFIG_H

#include <gtk/gtk.h>
#include "../callbacks/callbacks.hpp"
class SimpleCanvas;

namespace s21 {

class Config {
public:

    // Основная конфигурация GTK-окна и canvas
    static void config_GTK(GtkApplication* app, gpointer user_data);
private:

    // Обработчик закрытия окна
    static void on_window_destroy(GtkWidget* widget, gpointer data);

    // Добавляет все управляющие кнопки в контейнер
    static void addControls(GtkWidget* box, GtkWidget* window);

};

} // namespace s21

#endif // S21_CONFIG_H