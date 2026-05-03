#ifndef COLORDIALOG_APP
#define COLORDIALOG_APP

#include <gtk/gtk.h>
#include <string>
#include <functional>
#include "app_data.hpp"
#include "gui_globals.h"

class ColorDialog {
  public:
    using ColorSelectedCallback = std::function<void(double r, double g, double b, double a)>;

  private:
    GtkWindow* window;
    ColorSelectedCallback onColorSelectedCallback;

  private:
    bool active;
    static void onColorSelected(GObject *source, GAsyncResult *result, gpointer user_data);

  public:
    ColorDialog(GtkWindow* window_) : window(window_) {};
    ~ColorDialog() {};
    void setOnColorSelectedCallback(ColorSelectedCallback callback);
    bool isActive();
    void open(Rgb color);
};

#endif
