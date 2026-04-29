#ifndef GUI_APP
#define GUI_APP

#include <gtk/gtk.h>
#include "open_dialog.hpp"
#include "color_dialog.hpp"
#include "command.hpp"
#include "../simple_canvas.hpp"

struct AppData {
  private:
    Shape vertMode;    
  public:
    AppData() : vertMode(None) {}
    Shape getVertMode() const {
        return vertMode;
    }
    void setVertMode(Shape mode) {
        vertMode = mode;
    }
};

class GuiApp {
  private:
    AppData appData;
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

  public:    
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
    OpenDialog* openDialog;
    ColorDialog* colorDialog;
    GuiApp();
    ~GuiApp();
    int run(int argc, char **argv);
    void activate(GtkApplication *app);
    void colorSelected(double red, double green, double blue, double alpha);
    void openFileSelected(const std::string& path);
    AppData& getAppData();
    const AppData& getAppData() const;

  private:
    Command* openCommand;
    Command* saveCommand;
    Command* resetCommand;
    Command* rotateXCommand;
    Command* rotateYCommand;
    Command* rotateZCommand;
    Command* shiftCommand;
    Command* zoomCommand;
    Command* lineSwitchCommand;
    Command* projSwitchCommand;
    Command* fillSwitchCommand;
    Command* vertModeCommand;
    Command* colorCommand;
    void createCommands();

  public:
    void executeCommand(Command* cmd);
};

#endif
