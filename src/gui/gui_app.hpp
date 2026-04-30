#ifndef GUI_APP
#define GUI_APP

#include <gtk/gtk.h>
#include "open_dialog.hpp"
#include "color_dialog.hpp"
#include "command.hpp"
#include "simple_canvas.hpp"
#include "gui_globals.h"

struct AppData {
  private:
    std::string path;
    int angleX;
    int angleY;
    int angleZ;
    int shift;
    int zoom;
    bool lineSwitch;
    bool projSwitch;
    bool fillSwitch;
    Shape vertMode;
    Rgb color;
  public:
    AppData() {
      angleX = 0;
      angleY = 0;
      angleZ = 0;
      shift = 0;
      zoom = 0;
      lineSwitch = false;
      projSwitch = false;
      fillSwitch = false;
      vertMode = None;
      color = Rgb{0,0,0,0};
    }
    std::string getPath() const {
        return path;
    }
    int getAngleX() const {
        return angleX;
    }
    int getAngleY() const {
        return angleY;
    }
    int getAngleZ() const {
        return angleZ;
    }
    int getShift() const {
        return shift;
    }
    int getZoom() const {
        return zoom;
    }
    bool getLineSwitch() const {
        return lineSwitch;
    }
    bool getProjSwitch() const {
        return projSwitch;
    }
    bool getFillSwitch() const {
        return fillSwitch;
    }
    Shape getVertMode() const {
        return vertMode;
    }
    Rgb getColor() const {
        return color;
    }
    void setPath(std::string val) {
        path = val;
    }
    void setAngleX(int val) {
        angleX = val;
    }
    void setAngleY(int val) {
        angleY = val;
    }
    void setAngleZ(int val) {
        angleZ = val;
    }
    void setShift(int val) {
        shift = val;
    }
    void setZoom(int val) {
        zoom = val;
    }
    void setLineSwitch(bool val) {
        lineSwitch = val;
    }
    void setProjSwitch(bool val) {
        projSwitch = val;
    }
    void setFillSwitch(bool val) {
        fillSwitch = val;
    }
    void setVertMode(Shape mode) {
        vertMode = mode;
    }
    void setColor(Rgb color) {
        color = color;
    }
};

class GuiApp {
  private:
    AppData appData;
    static void onColorButtonClick(GtkButton* btn, gpointer user_data);
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
    SimpleCanvas* getCanvas();

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
