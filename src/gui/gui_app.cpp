#include "gui_app.hpp"

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <filesystem>
#include <iostream>

#include "app_data.hpp"
#include "color_dialog.hpp"
#include "open_dialog.hpp"

namespace fs = std::filesystem;

void GuiApp::onActivate(GtkApplication* app, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->activate(app);
    self->updateStatusBar();
}

void GuiApp::onOpenButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->openCommand);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->zoomCommand), self->getAppData()->getZoom());
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->scaleCommand), self->getAppData()->getScale());
}

void GuiApp::onResetButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->resetCommand);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->ySpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->xSpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->zSpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->shiftSpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->shiftVSpinnerButton), 0);
}

void GuiApp::onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->xSpinnerButton));
    self->getAppData()->setAngleX(value);
    self->executeCommand(self->rotateXCommand);
}

void GuiApp::onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->ySpinnerButton));
    self->getAppData()->setAngleY(value);
    self->executeCommand(self->rotateYCommand);
}

void GuiApp::onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zSpinnerButton));
    self->getAppData()->setAngleZ(value);
    self->executeCommand(self->rotateZCommand);
}

void GuiApp::onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftSpinnerButton));
    self->getAppData()->setShift(value);
    self->executeCommand(self->shiftCommand);
}

void GuiApp::onShiftVSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftVSpinnerButton));
    self->getAppData()->setShiftV(value);
    self->executeCommand(self->shiftVCommand);
}

void GuiApp::onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zoomSpinnerButton));
    self->getAppData()->setZoom(value);
    self->executeCommand(self->zoomCommand);
}

void GuiApp::onScaleSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->scaleSpinnerButton));
    self->getAppData()->setScale(value);
    self->executeCommand(self->scaleCommand);
}

void GuiApp::onLineSwitchActivate(GtkSwitch* sw, GParamSpec* pspec, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->lineSwitch));
    self->getAppData()->setLineSwitch(active);
    self->executeCommand(self->lineSwitchCommand);
}

void GuiApp::onProjSwitchActivate(GtkSwitch* sw, GParamSpec* pspec, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->projSwitch));
    self->getAppData()->setProjSwitch(active);
    self->executeCommand(self->projSwitchCommand);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->scaleSpinnerButton), self->getController()->getScale());
}

void GuiApp::onFillSwitchActivate(GtkSwitch* sw, GParamSpec* pspec, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->fillSwitch));
    self->getAppData()->setFillSwitch(active);
    self->executeCommand(self->fillSwitchCommand);
}

void GuiApp::onVertModeToggled(GtkCheckButton* btn, GParamSpec* pspec, gpointer user_data) {
    if (!gtk_check_button_get_active(btn)) {
        return;
    }
    Shape vertMode = Shape(GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "mode")));
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->getAppData()->setVertMode(vertMode);
    self->executeCommand(self->vertModeCommand);
}

void GuiApp::onColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->colorCommand);
}

void GuiApp::onBgColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->bgcolorCommand);
}

void GuiApp::onWeightSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->weightSpinnerButton));
    self->getAppData()->setWeight(value);
    self->executeCommand(self->weightCommand);
}

void GuiApp::onVertSizeSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    float value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->vertSizeButton));
    self->getAppData()->setVertSize(value);
    self->executeCommand(self->vertSizeCommand);
}

GuiApp::GuiApp(std::string ui, std::string sets) {
    ui_file = ui;
    settings_file = sets;
    createCommands();
    appData = new AppData(settings_file);
    app = gtk_application_new("com.application", G_APPLICATION_DEFAULT_FLAGS);
    c_ = new s21::Controller();
}

GuiApp::~GuiApp() {
    g_object_unref(app);
    delete appData;
    delete openDialog;
    delete colorDialog;
    delete c_;
    dropCommands();
}

int GuiApp::run(int argc, char** argv) {
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), this);
    return g_application_run(G_APPLICATION(app), argc, argv);
}

void GuiApp::activate(GtkApplication* app) {
    GtkBuilder* builder = gtk_builder_new_from_file(ui_file.c_str());

    window = gtk_builder_get_object(builder, "main");
    openButton = gtk_builder_get_object(builder, "open_button");
    resetButton = gtk_builder_get_object(builder, "reset_button");
    colorButton = gtk_builder_get_object(builder, "color_button");
    bgcolorButton = gtk_builder_get_object(builder, "bgcolor_button");
    xSpinnerButton = gtk_builder_get_object(builder, "rotate_x_spinner");
    ySpinnerButton = gtk_builder_get_object(builder, "rotate_y_spinner");
    zSpinnerButton = gtk_builder_get_object(builder, "rotate_z_spinner");
    shiftSpinnerButton = gtk_builder_get_object(builder, "shift_spinner");
    shiftVSpinnerButton = gtk_builder_get_object(builder, "shift_v_spinner");
    zoomSpinnerButton = gtk_builder_get_object(builder, "zoom_spinner");
    scaleSpinnerButton = gtk_builder_get_object(builder, "scale_spinner");
    paper = gtk_builder_get_object(builder, "paper");
    getCanvas(); //инициализируем виджет перед добавлением
    lineSwitch = gtk_builder_get_object(builder, "line_switch");
    projSwitch = gtk_builder_get_object(builder, "proj_switch");
    fillSwitch = gtk_builder_get_object(builder, "poli_fill_switch");
    noneModeCheck = gtk_builder_get_object(builder, "vert_mode_none");
    rectModeCheck = gtk_builder_get_object(builder, "vert_mode_rect");
    circModeCheck = gtk_builder_get_object(builder, "vert_mode_circ");
    status_vert = gtk_builder_get_object(builder, "status_vert");
    status_file = gtk_builder_get_object(builder, "status_file");
    status_edges = gtk_builder_get_object(builder, "status_edges");
    vertSizeButton = gtk_builder_get_object(builder, "vert_size_spinner");
    g_object_set_data(G_OBJECT(noneModeCheck), "mode", GINT_TO_POINTER(None));
    g_object_set_data(G_OBJECT(rectModeCheck), "mode", GINT_TO_POINTER(Rect));
    g_object_set_data(G_OBJECT(circModeCheck), "mode", GINT_TO_POINTER(Circle));
    weightSpinnerButton = gtk_builder_get_object(builder, "weight_spinner");
    statusLabel = gtk_builder_get_object(builder, "status_label");
    g_object_unref(builder);

    g_signal_connect(openButton, "clicked", G_CALLBACK(onOpenButtonClick), this);
    g_signal_connect(resetButton, "clicked", G_CALLBACK(onResetButtonClick), this);
    g_signal_connect(colorButton, "clicked", G_CALLBACK(onColorButtonClick), this);
    g_signal_connect(bgcolorButton, "clicked", G_CALLBACK(onBgColorButtonClick), this);
    g_signal_connect(xSpinnerButton, "value_changed", G_CALLBACK(onXSpinnerValueChanged), this);
    g_signal_connect(ySpinnerButton, "value_changed", G_CALLBACK(onYSpinnerValueChanged), this);
    g_signal_connect(zSpinnerButton, "value_changed", G_CALLBACK(onZSpinnerValueChanged), this);
    g_signal_connect(shiftSpinnerButton, "value_changed", G_CALLBACK(onShiftSpinnerValueChanged), this);
    g_signal_connect(shiftVSpinnerButton, "value_changed", G_CALLBACK(onShiftVSpinnerValueChanged), this);
    g_signal_connect(zoomSpinnerButton, "value_changed", G_CALLBACK(onZoomSpinnerValueChanged), this);
    g_signal_connect(scaleSpinnerButton, "value_changed", G_CALLBACK(onScaleSpinnerValueChanged), this);
    g_signal_connect(lineSwitch, "notify::active", G_CALLBACK(onLineSwitchActivate), this);
    g_signal_connect(projSwitch, "notify::active", G_CALLBACK(onProjSwitchActivate), this);
    g_signal_connect(fillSwitch, "notify::active", G_CALLBACK(onFillSwitchActivate), this);
    g_signal_connect(noneModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(rectModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(circModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(weightSpinnerButton, "value_changed", G_CALLBACK(onWeightSpinnerValueChanged), this);
    g_signal_connect(vertSizeButton, "value_changed", G_CALLBACK(onVertSizeSpinnerValueChanged), this);


    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    openDialog = new OpenDialog(GTK_WINDOW(window));
    openDialog->setOnFileSelected([this](const std::string& path) { this->openFileSelected(path); });

    colorDialog = new ColorDialog(GTK_WINDOW(window));

    gtk_label_set_text(GTK_LABEL(status_vert), "Вершины: 0");
    gtk_label_set_text(GTK_LABEL(status_edges), "Рёбра: 0");
    gtk_label_set_text(GTK_LABEL(status_file), "Файл: не выбран");

    restoreSett();
}

void GuiApp::restoreSett() {
    AppData* app = getAppData();
    app->loadFromFile(settings_file);
    SimpleCanvas* canvas = getCanvas();
    canvas->setBgColor(app->getBgColor());
    canvas->setVertColor(app->getColor());
    canvas->setLineWidth(app->getWeight());
    if (app->getPath() != "") {
        openFileSelected(app->getPath().c_str());
    }
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(weightSpinnerButton), app->getWeight());
}

void GuiApp::colorSelected(float red, float green, float blue, float alpha) {
    Rgb color = Rgb{red, green, blue, alpha};
    getAppData()->setColor(color);
    getCanvas()->setVertColor(color);
    getCanvas()->redraw();
}

void GuiApp::bgcolorSelected(float red, float green, float blue, float alpha) {
    Rgb color = Rgb{red, green, blue, alpha};
    getAppData()->setBgColor(color);
    getCanvas()->setBgColor(color);
    getCanvas()->redraw();
}

void GuiApp::openFileSelected(const std::string& path) {
    getAppData()->setPath(path);
    int w = gtk_widget_get_width(GTK_WIDGET(paper));
    int h = gtk_widget_get_height(GTK_WIDGET(paper));
    c_->loadFigure(path.c_str(), w, h);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(scaleSpinnerButton), c_->getScale());
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(zoomSpinnerButton), c_->getCameraZ());
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(shiftSpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(shiftVSpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(ySpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(xSpinnerButton), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(zSpinnerButton), 0);
    executeCommand(resetCommand);
    updateStatusBar();
    UpdateFigure();
    getCanvas()->redraw();
}

AppData* GuiApp::getAppData() { return appData; }

const AppData* GuiApp::getAppData() const { return appData; }

void GuiApp::createCommands() {
    this->openCommand = new OpenCommand(this);
    this->resetCommand = new ResetCommand(this);
    this->rotateXCommand = new RotateXCommand(this);
    this->rotateYCommand = new RotateYCommand(this);
    this->rotateZCommand = new RotateZCommand(this);
    this->shiftCommand = new ShiftCommand(this);
    this->shiftVCommand = new ShiftVCommand(this);
    this->zoomCommand = new ZoomCommand(this);
    this->scaleCommand = new ScaleCommand(this);
    this->lineSwitchCommand = new LineSwitchCommand(this);
    this->projSwitchCommand = new ProjSwitchCommand(this);
    this->fillSwitchCommand = new FillSwitchCommand(this);
    this->vertModeCommand = new VertModeCommand(this);
    this->colorCommand = new ColorCommand(this);
    this->bgcolorCommand = new BgColorCommand(this);
    this->weightCommand = new WeightCommand(this);
    this->vertSizeCommand = new VertSizeCommand(this);
}

void GuiApp::dropCommands() {
    delete this->openCommand;
    delete this->resetCommand;
    delete this->rotateXCommand;
    delete this->rotateYCommand;
    delete this->rotateZCommand;
    delete this->shiftCommand;
    delete this->shiftVCommand;
    delete this->zoomCommand;
    delete this->scaleCommand;
    delete this->lineSwitchCommand;
    delete this->projSwitchCommand;
    delete this->fillSwitchCommand;
    delete this->vertModeCommand;
    delete this->colorCommand;
    delete this->bgcolorCommand;
    delete this->weightCommand;
    delete this->vertSizeCommand;
}

void GuiApp::executeCommand(Command* cmd) {
    if (cmd != nullptr) {
        cmd->execute();
        int w = gtk_widget_get_width(GTK_WIDGET(paper));
        int h = gtk_widget_get_height(GTK_WIDGET(paper));
        if (w==0 || h== 0) return;
        s21::matrix_t mvp_matrix = c_->getMVP(w/h);
        std::vector<GLfloat> mvp; 
        for (size_t i = 0; i<4; i++){
            for (size_t x = 0; x<4; x++){
                mvp.push_back(mvp_matrix[x][i]);
            }
        }
        getCanvas()->setMVP(mvp.data());
        getCanvas()->redraw();
    }
}

SimpleCanvas* GuiApp::getCanvas() {
    return SimpleCanvas::GetInstance(GTK_WIDGET(paper));
}

s21::Controller* GuiApp::getController() { return c_; }

void GuiApp::updateStatusBar() {
    std::string vert_text = "Вершины: " + std::to_string(c_->getVerticesNum());
    std::string edges_text = "Грани: " + std::to_string(c_->getEdgesNum());
    std::string file_text = "Файл: " + getAppData()->getPath();

    gtk_label_set_text(GTK_LABEL(status_vert), vert_text.c_str());
    gtk_label_set_text(GTK_LABEL(status_edges), edges_text.c_str());
    gtk_label_set_text(GTK_LABEL(status_file), file_text.c_str());
}

s21::Poly_Proj_t GuiApp::getFigure() { return c_->getFigure(); }

s21::Bounds GuiApp::getFigureBounds() { return c_->getFigureBounds(); }

void GuiApp::UpdateFigure(){
    s21::matrix_t m = c_->getMatrix();
    std::vector<GLfloat> vertices = {};
    std::vector<GLfloat> cube_vertices = {};
    std::vector<GLuint> indices = {};
    for (auto row : m){
        vertices.push_back(row[0]);
        vertices.push_back(row[1]);
        vertices.push_back(row[2]);
        vertices.push_back(0); //uv надо правильные поставить
        vertices.push_back(0); //uv надо правильные поставить

        cube_vertices.push_back(row[0]);
        cube_vertices.push_back(row[1]);
        cube_vertices.push_back(row[2]);
    }
    s21::Poly_t polies = c_->getPolygons();
    for (auto poly : polies){
        for (auto i : poly){
            indices.push_back(i);
        }

    }
    getCanvas()->updateFigure(vertices, indices, cube_vertices);
}

