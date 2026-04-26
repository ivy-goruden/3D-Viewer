#ifndef GUI_APP
#define GUI_APP

#include <gtk/gtk.h>
#include "../simple_canvas.hpp"

class GuiApp {
  private:
    GtkApplication *app;
    GObject* window;
    GObject* openButton;
    GObject* saveButton;
    GObject* resetButton;
    GObject* xSpinnerButton;
    GObject* ySpinnerButton;
    GObject* zSpinnerButton;
    GObject* shiftSpinnerButton;
    GObject* zoomSpinnerButton;
    GObject* paper;
    static void onActivate(GtkApplication *app, gpointer user_data);
    static void onOpenButtonClick(GtkButton* btn, gpointer user_data);
    static void onSaveButtonClick(GtkButton* btn, gpointer user_data);
    static void onResetButtonClick(GtkButton* btn, gpointer user_data);
    static void onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);

  public:
    GuiApp();
    ~GuiApp();
    int run(int argc, char **argv);
    void activate(GtkApplication *app);
    void openButtonClick(GtkButton* btn);
    void saveButtonClick(GtkButton* btn);
    void resetButtonClick(GtkButton* btn);
    void xSpinnerValueChanged(GtkSpinButton* btn);
    void ySpinnerValueChanged(GtkSpinButton* btn);
    void zSpinnerValueChanged(GtkSpinButton* btn);
    void shiftSpinnerValueChanged(GtkSpinButton* btn);
    void zoomSpinnerValueChanged(GtkSpinButton* btn);
};

#endif
