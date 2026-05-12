#include "gui.hpp"
#include <gtk/gtk.h>
#include <iostream>
#include <filesystem>
#include <format>

void DrawSomething(cairo_t* cr, GuiApp* gui) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_line_width(cr, 1);
    cairo_arc(cr, 0, 0, 100, 0, 2 * M_PI);
    cairo_stroke(cr);

    gui->drawLine(cr, gui->projectVertex(gui->shape[0]), gui->projectVertex(gui->shape[1]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[1]), gui->projectVertex(gui->shape[2]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[2]), gui->projectVertex(gui->shape[3]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[3]), gui->projectVertex(gui->shape[0]));

    gui->drawLine(cr, gui->projectVertex(gui->shape[4]), gui->projectVertex(gui->shape[5]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[5]), gui->projectVertex(gui->shape[6]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[6]), gui->projectVertex(gui->shape[7]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[7]), gui->projectVertex(gui->shape[4]));

    gui->drawLine(cr, gui->projectVertex(gui->shape[0]), gui->projectVertex(gui->shape[4]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[1]), gui->projectVertex(gui->shape[5]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[2]), gui->projectVertex(gui->shape[6]));
    gui->drawLine(cr, gui->projectVertex(gui->shape[3]), gui->projectVertex(gui->shape[7]));
}

s21::Point GuiApp::viewportToCanvas(double x, double y) {
    return toScreenPoint(s21::Point{x * Cw / Vw, y * Ch / Vh});
}

s21::Point GuiApp::projectVertex(s21::Point3d v) {
    return viewportToCanvas(v.x * d / v.z, v.y * d / v.z);
}

void GuiApp::drawLine(cairo_t* cr, s21::Point p1, s21::Point p2) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_move_to(cr, p1.x, p1.y);
    cairo_line_to(cr, p2.x, p2.y);
    cairo_stroke(cr);
}

s21::Point GuiApp::toScreenPoint(s21::Point canvasPoint) {
    return s21::Point{Cw / 2 + canvasPoint.x, Ch / 2 - canvasPoint.y};
}

s21::Point GuiApp::toCanvasPoint(s21::Point screenPoint) {
    return s21::Point{-Cw / 2 + screenPoint.x, Ch / 2 - screenPoint.y};
}

GuiApp::GuiApp() {
    d = 1;
    Cw = 1;
    Ch = 1;
    Vw = 1;
    Vh = 1;

    shape.push_back({-1,  1,  1});
    shape.push_back({ 1,  1,  1});
    shape.push_back({ 1, -1,  1});
    shape.push_back({-1, -1,  1});
    shape.push_back({-1,  1,  2});
    shape.push_back({ 1,  1,  2});
    shape.push_back({ 1, -1,  2});
    shape.push_back({-1, -1,  2});

    app = gtk_application_new("com.application", G_APPLICATION_DEFAULT_FLAGS);
}

GuiApp::~GuiApp() {
    g_object_unref(app);
}

int GuiApp::run(int argc, char **argv) {
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), this);
    return g_application_run(G_APPLICATION(app), argc, argv);
}

void GuiApp::onActivate(GtkApplication *app, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->activate(app);
}

void GuiApp::onOpenButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onSaveButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onResetButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->xSpinnerButton));
    self->d = value;
    self->redraw();
    std::cout << "[onXSpinnerValueChanged] " << std::format("v{}\n", value);
}

void GuiApp::onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->ySpinnerButton));
}

void GuiApp::onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zSpinnerButton));
}

void GuiApp::onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftSpinnerButton));
}

void GuiApp::onShiftVSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftVSpinnerButton));
}

void GuiApp::onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zoomSpinnerButton));
}

void GuiApp::onLineSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->lineSwitch));
}

void GuiApp::onProjSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->projSwitch));
}

void GuiApp::onFillSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->fillSwitch));
}

void GuiApp::onVertModeToggled(GtkCheckButton* btn, GParamSpec *pspec, gpointer user_data) {
    if (!gtk_check_button_get_active(btn)) {
        return;
    }
    int vertMode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "mode"));
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onBgColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onWeightSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->weightSpinnerButton));
}

void GuiApp::activate(GtkApplication* app) {
    GtkBuilder* builder = gtk_builder_new_from_file(ui_file.c_str());
    window = gtk_builder_get_object(builder, "main");
    openButton = gtk_builder_get_object(builder, "open_button");
    saveButton = gtk_builder_get_object(builder, "save_button");
    resetButton = gtk_builder_get_object(builder, "reset_button");
    colorButton = gtk_builder_get_object(builder, "color_button");
    bgcolorButton = gtk_builder_get_object(builder, "bgcolor_button");
    xSpinnerButton = gtk_builder_get_object(builder, "rotate_x_spinner");
    ySpinnerButton = gtk_builder_get_object(builder, "rotate_y_spinner");
    zSpinnerButton = gtk_builder_get_object(builder, "rotate_z_spinner");
    shiftSpinnerButton = gtk_builder_get_object(builder, "shift_spinner");
    shiftVSpinnerButton = gtk_builder_get_object(builder, "shift_v_spinner");
    zoomSpinnerButton = gtk_builder_get_object(builder, "zoom_spinner");
    weightSpinnerButton = gtk_builder_get_object(builder, "weight_spinner");
    paper = gtk_builder_get_object(builder, "paper");
    status_vert = gtk_builder_get_object(builder, "status_vert");
    status_file = gtk_builder_get_object(builder, "status_file");
    status_edges = gtk_builder_get_object(builder, "status_edges");
    statusLabel = gtk_builder_get_object(builder, "status_label");
    g_object_unref(builder);

    g_signal_connect(openButton, "clicked", G_CALLBACK(onOpenButtonClick), this);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(onSaveButtonClick), this);
    g_signal_connect(resetButton, "clicked", G_CALLBACK(onResetButtonClick), this);
    g_signal_connect(colorButton, "clicked", G_CALLBACK(onColorButtonClick), this);
    g_signal_connect(bgcolorButton, "clicked", G_CALLBACK(onBgColorButtonClick), this);
    g_signal_connect(xSpinnerButton, "value_changed", G_CALLBACK(onXSpinnerValueChanged), this);
    g_signal_connect(ySpinnerButton, "value_changed", G_CALLBACK(onYSpinnerValueChanged), this);
    g_signal_connect(zSpinnerButton, "value_changed", G_CALLBACK(onZSpinnerValueChanged), this);
    g_signal_connect(shiftSpinnerButton, "value_changed", G_CALLBACK(onShiftSpinnerValueChanged), this);
    g_signal_connect(shiftVSpinnerButton, "value_changed", G_CALLBACK(onShiftVSpinnerValueChanged), this);
    g_signal_connect(zoomSpinnerButton, "value_changed", G_CALLBACK(onZoomSpinnerValueChanged), this);
    g_signal_connect(weightSpinnerButton, "value_changed", G_CALLBACK(onWeightSpinnerValueChanged), this);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(paper), on_draw_static, this, nullptr);
}

void GuiApp::on_draw_static(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->onDraw(cr, width, height);
}

void GuiApp::onDraw(cairo_t* cr, int width, int height) {
    Cw = width;
    Ch = height;
    Vw = Cw;
    Vh = Ch;
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    cairo_set_line_width(cr, 10);
    cairo_translate(cr, 0, 0);
    cairo_scale(cr, 1, 1);
    DrawSomething(cr, this);
    std::cout << "[onDraw] " << std::format("w{},h{}\n", width, height);
}

void GuiApp::redraw() {
    gtk_widget_queue_draw(GTK_WIDGET(paper));
}
