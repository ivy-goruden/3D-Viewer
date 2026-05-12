#ifndef GUI
#define GUI

#include <gtk/gtk.h>
#include <string>
#include "../include/globals.h"

class GuiApp {
  private:
    static void onColorButtonClick(GtkButton* btn, gpointer user_data);
    static void onBgColorButtonClick(GtkButton* btn, gpointer user_data);
    static void onActivate(GtkApplication *app, gpointer user_data);
    static void onOpenButtonClick(GtkButton* btn, gpointer user_data);
    static void onSaveButtonClick(GtkButton* btn, gpointer user_data);
    static void onResetButtonClick(GtkButton* btn, gpointer user_data);
    static void onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onShiftVSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onLineSwitchActivate(GtkSwitch* swtch, GParamSpec *pspec, gpointer user_data);
    static void onProjSwitchActivate(GtkSwitch* swtch, GParamSpec *pspec, gpointer user_data);
    static void onFillSwitchActivate(GtkSwitch* swtch, GParamSpec *pspec, gpointer user_data);
    static void onVertModeToggled(GtkCheckButton* btn, GParamSpec *pspec, gpointer user_data);
    static void onWeightSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data);
    static void onStatusUpdate();

  public:
    std::string ui_file;
    GtkApplication *app;
    GObject* window;
    GObject* openButton;
    GObject* saveButton;
    GObject* resetButton;
    GObject* xSpinnerButton;
    GObject* ySpinnerButton;
    GObject* zSpinnerButton;
    GObject* shiftSpinnerButton;
    GObject* shiftVSpinnerButton;
    GObject* zoomSpinnerButton;
    GObject* paper;
    GObject* lineSwitch;
    GObject* projSwitch;
    GObject* fillSwitch;
    GObject* noneModeCheck;
    GObject* rectModeCheck;
    GObject* circModeCheck;
    GObject* colorButton;
    GObject* bgcolorButton;
    GObject* weightSpinnerButton;
    GObject* statusLabel;
    GObject* status_vert;
    GObject* status_file;
    GObject* status_edges;
    GuiApp();
    ~GuiApp();
    int run(int argc, char **argv);
    void activate(GtkApplication *app);
    void onDraw(cairo_t* cr, int width, int height);
    void redraw();
    static void on_draw_static(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data);

  public:
    int d, Cw, Ch, Vw, Vh;
    double ratio;
    std::vector<s21::Point3d> shape;
    s21::Point viewportToCanvas(double x, double y);
    s21::Point projectVertex(s21::Point3d v);
    void drawLine(cairo_t* cr, s21::Point p1, s21::Point p2);
    s21::Point toScreenPoint(s21::Point canvasPoint);
    s21::Point toCanvasPoint(s21::Point screenPoint);
};

#endif
