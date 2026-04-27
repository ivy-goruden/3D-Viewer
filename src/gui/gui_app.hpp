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
    GObject* lineSwitch;
    GObject* projSwitch;
    GObject* fillSwitch;
    GObject* noneModeCheck;
    GObject* rectModeCheck;
    GObject* circModeCheck;
    GObject* colorButton;
    static void onColorButtonClick(GtkButton* btn, gpointer user_data);
    static void onColorSelected(GObject *source, GAsyncResult *result, gpointer user_data);
    static void onActivate(GtkApplication *app, gpointer user_data);
    static void onOpenButtonClick(GtkButton* btn, gpointer user_data);
    static void onSaveButtonClick(GtkButton* btn, gpointer user_data);
    static void onResetButtonClick(GtkButton* btn, gpointer user_data);
    static void onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onLineSwitchActivate(GtkSwitch* swtch, GParamSpec *pspec, gpointer user_data);
    static void onProjSwitchActivate(GtkSwitch* swtch, GParamSpec *pspec, gpointer user_data);
    static void onFillSwitchActivate(GtkSwitch* swtch, GParamSpec *pspec, gpointer user_data);
    static void onVertModeToggled(GtkCheckButton* btn, GParamSpec *pspec, gpointer user_data);
    static void onOpenFileSelected(GtkButton* btn, gpointer user_data);

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
    void lineSwitchActivate(GtkSwitch* sw);
    void projSwitchActivate(GtkSwitch* sw);
    void fillSwitchActivate(GtkSwitch* sw);
    void vertModeToggled(GtkCheckButton* btn, Shape vertMode);
    void colorButtonClick(GtkButton* btn);
    void colorSelect(double red, double green, double blue, double alpha);
    void openFileSelect(std::string path);
};

#endif
