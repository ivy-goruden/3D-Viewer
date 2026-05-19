#ifndef GUI
#define GUI

#include <time.h>
#include <gtk/gtk.h>
#include <string>
#include "../include/globals.h"

class GuiApp;

struct KeyData {
    GuiApp* app;
    GtkWidget* drawing_area;
    gboolean is_ctrl;
    gboolean is_alt;
    struct timespec press_time;
};

struct DragData {
    GuiApp* app;
    GtkWidget* drawing_area;
    double drag_start_x;
    double drag_start_y;
    double offset_x;
    double offset_y;
    gboolean is_dragging;
};

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
    DragData *drag_data;
    KeyData *key_data;
    GuiApp();
    ~GuiApp();
    int run(int argc, char **argv);
    void activate(GtkApplication *app);
    void onDraw(cairo_t* cr, int width, int height);
    void redraw();
    static void on_draw_static(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data);
    static void on_motion(GtkEventControllerMotion *controller, gdouble x, gdouble y, gpointer user_data);
    static void on_drag_begin(GtkGestureDrag *gesture, double start_x, double start_y, gpointer user_data);
    static void on_drag_update(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer user_data);
    static void on_drag_end(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer user_data);
    static gboolean on_scroll(GtkEventControllerScroll *controller, gdouble dx, gdouble dy, gpointer user_data);
    static gboolean on_key_pressed(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data);
    static gboolean on_key_released(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data);

    void onDragUpdate();
    void onDragEnd();
    void onScroll(double dx, double dy);

  public:
    int d, Cw, Ch, Vw, Vh;
    double x, y;
    double ratio;
    s21::Transform tr;
    s21::matrix_t shapeVert;
    s21::Poly_t shapeFace;    
    s21::Point viewportToCanvas(double x, double y);
    s21::Point projectVertex(s21::Point3d v);
    void drawLine(cairo_t* cr, s21::Point p1, s21::Point p2);
    void drawDot(cairo_t* cr, s21::Point p);
    void drawObject(cairo_t* cr, s21::matrix_t& verts, s21::Poly_t& faces, s21::Transform tr);
    void drawFace(cairo_t* cr, std::vector<int> face, s21::Vert_t& projected);
    s21::Point toScreenPoint(s21::Point canvasPoint);
    s21::Point toCanvasPoint(s21::Point screenPoint);
    s21::Point3d vectorToPoint3d(std::vector<double> v);
    s21::matrix_t applyTransform(s21::matrix_t& verts, s21::Transform tr);

};

#endif
