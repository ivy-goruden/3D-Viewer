#include "gui_app.hpp"
#include <gtk/gtk.h>
#include <iostream>
#include <filesystem>
#include "open_dialog.hpp"
#include "color_dialog.hpp"
#include "app_data.hpp"

namespace fs = std::filesystem;

void GuiApp::onActivate(GtkApplication *app, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->activate(app);
    self->createCommands();
    self->updateStatusBar();
}

void GuiApp::onOpenButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->openCommand);
}

void GuiApp::onSaveButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->saveCommand);
}

void GuiApp::onResetButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->resetCommand);
}

void GuiApp::onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->xSpinnerButton));
    self->getAppData().setAngleX(value);
    self->executeCommand(self->rotateXCommand);
}

void GuiApp::onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->ySpinnerButton));
    self->getAppData().setAngleY(value);
    self->executeCommand(self->rotateYCommand);
}

void GuiApp::onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zSpinnerButton));
    self->getAppData().setAngleZ(value);
    self->executeCommand(self->rotateZCommand);
}

void GuiApp::onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftSpinnerButton));
    self->getAppData().setShift(value);
    self->executeCommand(self->shiftCommand);
}

void GuiApp::onShiftVSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftVSpinnerButton));
    self->getAppData().setShiftV(value);
    self->executeCommand(self->shiftCommand);
}

void GuiApp::onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zoomSpinnerButton));
    self->getAppData().setZoom(value);
    self->executeCommand(self->zoomCommand);
}

void GuiApp::onLineSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->lineSwitch));
    self->getAppData().setLineSwitch(active);
    self->executeCommand(self->lineSwitchCommand);
}

void GuiApp::onProjSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->projSwitch));
    self->getAppData().setProjSwitch(active);
    self->executeCommand(self->projSwitchCommand);
}

void GuiApp::onFillSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->fillSwitch));
    self->getAppData().setFillSwitch(active);
    self->executeCommand(self->fillSwitchCommand);
}

void GuiApp::onVertModeToggled(GtkCheckButton* btn, GParamSpec *pspec, gpointer user_data) {
    if (!gtk_check_button_get_active(btn)) {
        return;
    }
    Shape vertMode = Shape(GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "mode")));
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->getAppData().setVertMode(vertMode);
    self->executeCommand(self->vertModeCommand);
}

void GuiApp::onColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->colorCommand);
}

void GuiApp::onBgColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->bgcolorCommand);
}

void GuiApp::onWeightSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->weightSpinnerButton));
    self->getAppData().setWeight(value);
    self->executeCommand(self->weightCommand);    
    self->updateStatusBar();
}

GuiApp::GuiApp() {
    app = gtk_application_new("com.application", G_APPLICATION_DEFAULT_FLAGS);
}

GuiApp::~GuiApp() {
    g_object_unref(app);
}

int GuiApp::run(int argc, char **argv) {
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), this);
    return g_application_run(G_APPLICATION(app), argc, argv);
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
    paper = gtk_builder_get_object(builder, "paper");
    lineSwitch = gtk_builder_get_object(builder, "line_switch");
    projSwitch = gtk_builder_get_object(builder, "proj_switch");
    fillSwitch = gtk_builder_get_object(builder, "poli_fill_switch");
    noneModeCheck = gtk_builder_get_object(builder, "vert_mode_none");
    rectModeCheck = gtk_builder_get_object(builder, "vert_mode_rect");
    circModeCheck = gtk_builder_get_object(builder, "vert_mode_circ");
    status_vert = gtk_builder_get_object(builder, "status_vert");
    status_file = gtk_builder_get_object(builder, "status_file");
    status_edges = gtk_builder_get_object(builder, "status_edges");
    g_object_set_data(G_OBJECT(noneModeCheck), "mode", GINT_TO_POINTER(None));
    g_object_set_data(G_OBJECT(rectModeCheck), "mode", GINT_TO_POINTER(Rect));
    g_object_set_data(G_OBJECT(circModeCheck), "mode", GINT_TO_POINTER(Circle));
    weightSpinnerButton = gtk_builder_get_object(builder, "weight_spinner");
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
    g_signal_connect(lineSwitch, "notify::active", G_CALLBACK(onLineSwitchActivate), this);
    g_signal_connect(projSwitch, "notify::active", G_CALLBACK(onProjSwitchActivate), this);
    g_signal_connect(fillSwitch, "notify::active", G_CALLBACK(onFillSwitchActivate), this);
    g_signal_connect(noneModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(rectModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(circModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(weightSpinnerButton, "value_changed", G_CALLBACK(onWeightSpinnerValueChanged), this);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    openDialog = new OpenDialog(GTK_WINDOW(window));
    openDialog->setOnFileSelected([this](const std::string& path) {
        this->openFileSelected(path);
    });

    colorDialog = new ColorDialog(GTK_WINDOW(window));

    auto canvas_shared = std::make_shared<SimpleCanvas>(GTK_WIDGET(paper));    
    // Сохраняем shared_ptr в куче, чтобы передать в колбэки
    auto* canvas_ptr = new std::shared_ptr<SimpleCanvas>(canvas_shared);    
    // Привязываем к окну, чтобы удалить shared_ptr при уничтожении окна
    g_object_set_data_full(G_OBJECT(window), "canvas", canvas_ptr,
        [](gpointer data) {
            delete static_cast<std::shared_ptr<SimpleCanvas>*>(data);
        });

    gtk_label_set_text(GTK_LABEL(status_vert), "Вершины: 0");
    gtk_label_set_text(GTK_LABEL(status_edges), "Рёбра: 0");
    gtk_label_set_text(GTK_LABEL(status_file), "Файл: не выбран");
}

void GuiApp::colorSelected(double red, double green, double blue, double alpha) {
    Rgb color = Rgb{red, green, blue, alpha};
    getAppData().setColor(color);
    getCanvas()->setVertColor(color);
    getCanvas()->redraw();
}

void GuiApp::bgcolorSelected(double red, double green, double blue, double alpha) {
    Rgb color = Rgb{red, green, blue, alpha};
    getAppData().setBgColor(color);
    getCanvas()->setBgColor(color);
    getCanvas()->redraw();
}

void GuiApp::openFileSelected(const std::string& path) {
    g_print("%s\n", path.c_str());
    getAppData().setPath(path);
    auto* canvas =  getCanvas();
    canvas->loadFigure(path.c_str());
    canvas->redraw();
    updateStatusBar();
}

AppData& GuiApp::getAppData() {
    return appData;
}

const AppData& GuiApp::getAppData() const {
    return appData;
}

void GuiApp::createCommands() {
    this->openCommand = new OpenCommand(this);
    this->saveCommand = new SaveCommand(this);
    this->resetCommand = new ResetCommand(this);
    this->rotateXCommand = new RotateXCommand(this);
    this->rotateYCommand = new RotateYCommand(this);
    this->rotateZCommand = new RotateZCommand(this);
    this->shiftCommand = new ShiftCommand(this);
    this->zoomCommand = new ZoomCommand(this);
    this->lineSwitchCommand = new LineSwitchCommand(this);
    this->projSwitchCommand = new ProjSwitchCommand(this);
    this->fillSwitchCommand = new FillSwitchCommand(this);
    this->vertModeCommand = new VertModeCommand(this);
    this->colorCommand = new ColorCommand(this);
    this->bgcolorCommand = new BgColorCommand(this);
    this->weightCommand = new WeightCommand(this);
}

void GuiApp::executeCommand(Command *cmd) {
    if (cmd) {
        cmd->execute();
    }
}

SimpleCanvas* GuiApp::getCanvas(){
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
    g_object_get_data(window, "canvas"));
    return canvas ? canvas->get() : nullptr;
}

void GuiApp::updateStatusBar(){
    SimpleCanvas* canvas = getCanvas();
    if (!canvas) return;

    std::string vert_text = "Вершины: " + std::to_string(canvas->getVerticesNum());
    std::string edges_text = "Грани: " + std::to_string(canvas->getEdgesNum());
    std::string file_text = "Файл: " + getAppData().getPath();

    gtk_label_set_text(GTK_LABEL(status_vert), vert_text.c_str());
    gtk_label_set_text(GTK_LABEL(status_edges), edges_text.c_str());
    gtk_label_set_text(GTK_LABEL(status_file), file_text.c_str());
}
