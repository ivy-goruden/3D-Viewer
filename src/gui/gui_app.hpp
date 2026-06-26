#ifndef GUI_APP
#define GUI_APP

#include <gtk/gtk.h>

#include "app_data.hpp"
#include "color_dialog.hpp"
#include "command.hpp"
#include "gui_globals.h"
#include "open_dialog.hpp"
#include "simple_canvas.hpp"

class GuiApp {
   private:
    AppData* appData;
    static void onDotColorButtonClick(GtkButton* btn, gpointer user_data);
    static void onPolyColorButtonClick(GtkButton* btn, gpointer user_data);
    static void onBgColorButtonClick(GtkButton* btn, gpointer user_data);
    static void onActivate(GtkApplication* app, gpointer user_data);
    static void onOpenButtonClick(GtkButton* btn, gpointer user_data);
    static void onResetButtonClick(GtkButton* btn, gpointer user_data);
    static void onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onShiftVSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onScaleSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onLineSwitchActivate(GtkSwitch* swtch, GParamSpec* pspec, gpointer user_data);
    static void onProjSwitchActivate(GtkSwitch* swtch, GParamSpec* pspec, gpointer user_data);
    static void onFillSwitchActivate(GtkSwitch* swtch, GParamSpec* pspec, gpointer user_data);
    static void onVertModeToggled(GtkCheckButton* btn, GParamSpec* pspec, gpointer user_data);
    static void onWeightSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onVertSizeSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onStatusUpdate();
    void restoreSett();

   public:
    std::string ui_file;
    std::string settings_file;
    GtkApplication* app;
    GObject* window;
    GObject* openButton;
    GObject* resetButton;
    GObject* xSpinnerButton;
    GObject* ySpinnerButton;
    GObject* zSpinnerButton;
    GObject* shiftSpinnerButton;
    GObject* shiftVSpinnerButton;
    GObject* zoomSpinnerButton;
    GObject* scaleSpinnerButton;
    GObject* paper;
    GObject* lineSwitch;
    GObject* projSwitch;
    GObject* fillSwitch;
    GObject* noneModeCheck;
    GObject* rectModeCheck;
    GObject* circModeCheck;
    GObject* dotColorButton;
    GObject* bgcolorButton;
    GObject* figureColorButton;
    GObject* weightSpinnerButton;
    GObject* statusLabel;
    OpenDialog* openDialog;
    ColorDialog* colorDialog;
    GObject* status_vert;
    GObject* status_file;
    GObject* status_edges;
    GObject* vertSizeButton;
    s21::Controller* c_;
    GuiApp(std::string, std::string);
    ~GuiApp();
    int run(int argc, char** argv);
    void activate(GtkApplication* app);
    void dotColorSelected(float red, float green, float blue, float alpha);
    void polyColorSelected(float red, float green, float blue, float alpha);
    void bgcolorSelected(float red, float green, float blue, float alpha);
    void openFileSelected(const std::string& path);
    void updateStatusBar();
    //s21::Poly_Proj_t getFigure();
    AppData* getAppData();
    const AppData* getAppData() const;
    SimpleCanvas* getCanvas();
    s21::Controller* getController();
    s21::Bounds getFigureBounds();

   private:
    Command* openCommand;
    Command* resetCommand;
    Command* rotateXCommand;
    Command* rotateYCommand;
    Command* rotateZCommand;
    Command* shiftCommand;
    Command* shiftVCommand;
    Command* zoomCommand;
    Command* scaleCommand;
    Command* lineSwitchCommand;
    Command* projSwitchCommand;
    Command* fillSwitchCommand;
    Command* vertModeCommand;
    Command* dotColorCommand;
    Command* bgcolorCommand;
    Command* weightCommand;
    Command* vertSizeCommand;
    Command* polyColorCommand;
    void createCommands();
    void dropCommands();
    void UpdateFigure();

   public:
    void executeCommand(Command* cmd);
};

#endif
