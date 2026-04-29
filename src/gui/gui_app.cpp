#include "gui_app.hpp"
#include <gtk/gtk.h>
#include <iostream>
#include "open_dialog.hpp"

void GuiApp::onActivate(GtkApplication *app, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->activate(app);
    self->createCommands();
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

void GuiApp::onColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->colorButtonClick(btn);
}

void GuiApp::onColorSelected(GObject *source, GAsyncResult *result, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    
    GtkColorDialog *dialog = GTK_COLOR_DIALOG(source);
    GError *error = NULL;    
    GdkRGBA *color = gtk_color_dialog_choose_rgba_finish(dialog, result, &error);
    if (color) {
        self->colorSelect(color->red, color->green, color->blue, color->alpha);
        g_free(color);
    } else if (error) {
        if (!g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_CANCELLED)) {
            g_printerr("Ошибка: %s\n", error->message);
        }
        g_error_free(error);
    }
}

void GuiApp::onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->executeCommand(self->rotateXCommand);
}

void GuiApp::onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->ySpinnerValueChanged(btn);
}

void GuiApp::onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->zSpinnerValueChanged(btn);
}

void GuiApp::onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->shiftSpinnerValueChanged(btn);
}

void GuiApp::onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->zoomSpinnerValueChanged(btn);
}

void GuiApp::onLineSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->lineSwitchActivate(sw);
}

void GuiApp::onProjSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->projSwitchActivate(sw);
}

void GuiApp::onFillSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->fillSwitchActivate(sw);
}

void GuiApp::onVertModeToggled(GtkCheckButton* btn, GParamSpec *pspec, gpointer user_data) {
    if (!gtk_check_button_get_active(btn)) {
        return;
    }
    GuiApp *self = static_cast<GuiApp*>(user_data);
    Shape vertMode = Shape(GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "mode")));
    self->vertModeToggled(btn, vertMode);
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
    GtkBuilder* builder = gtk_builder_new_from_file("gui/3dviewer.ui");
    window = gtk_builder_get_object(builder, "main");
    openButton = gtk_builder_get_object(builder, "open_button");
    saveButton = gtk_builder_get_object(builder, "save_button");
    resetButton = gtk_builder_get_object(builder, "reset_button");
    colorButton = gtk_builder_get_object(builder, "color_button");
    xSpinnerButton = gtk_builder_get_object(builder, "rotate_x_spinner");
    ySpinnerButton = gtk_builder_get_object(builder, "rotate_y_spinner");
    zSpinnerButton = gtk_builder_get_object(builder, "rotate_z_spinner");
    shiftSpinnerButton = gtk_builder_get_object(builder, "shift_spinner");
    zoomSpinnerButton = gtk_builder_get_object(builder, "zoom_spinner");
    paper = gtk_builder_get_object(builder, "paper");
    lineSwitch = gtk_builder_get_object(builder, "line_switch");
    projSwitch = gtk_builder_get_object(builder, "proj_switch");
    fillSwitch = gtk_builder_get_object(builder, "poli_fill_switch");
    noneModeCheck = gtk_builder_get_object(builder, "vert_mode_none");
    rectModeCheck = gtk_builder_get_object(builder, "vert_mode_rect");
    circModeCheck = gtk_builder_get_object(builder, "vert_mode_circ");
    g_object_set_data(G_OBJECT(noneModeCheck), "mode", GINT_TO_POINTER(None));
    g_object_set_data(G_OBJECT(rectModeCheck), "mode", GINT_TO_POINTER(Rect));
    g_object_set_data(G_OBJECT(circModeCheck), "mode", GINT_TO_POINTER(Circle));
    g_object_unref(builder);

    g_signal_connect(openButton, "clicked", G_CALLBACK(onOpenButtonClick), this);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(onSaveButtonClick), this);
    g_signal_connect(resetButton, "clicked", G_CALLBACK(onResetButtonClick), this);
    g_signal_connect(colorButton, "clicked", G_CALLBACK(onColorButtonClick), this);
    g_signal_connect(xSpinnerButton, "value_changed", G_CALLBACK(onXSpinnerValueChanged), this);
    g_signal_connect(ySpinnerButton, "value_changed", G_CALLBACK(onYSpinnerValueChanged), this);
    g_signal_connect(zSpinnerButton, "value_changed", G_CALLBACK(onZSpinnerValueChanged), this);
    g_signal_connect(shiftSpinnerButton, "value_changed", G_CALLBACK(onShiftSpinnerValueChanged), this);
    g_signal_connect(zoomSpinnerButton, "value_changed", G_CALLBACK(onZoomSpinnerValueChanged), this);
    g_signal_connect(lineSwitch, "notify::active", G_CALLBACK(onLineSwitchActivate), this);
    g_signal_connect(projSwitch, "notify::active", G_CALLBACK(onProjSwitchActivate), this);
    g_signal_connect(fillSwitch, "notify::active", G_CALLBACK(onFillSwitchActivate), this);
    g_signal_connect(noneModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(rectModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);
    g_signal_connect(circModeCheck, "notify::active", G_CALLBACK(onVertModeToggled), this);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    openDialog = new OpenDialog(GTK_WINDOW(window));
    openDialog->setOnFileSelected([this](const std::string& path) {
        this->openFileSelected(path);
    });

    auto canvas_shared = std::make_shared<SimpleCanvas>(GTK_WIDGET(paper));    
    // Сохраняем shared_ptr в куче, чтобы передать в колбэки
    auto* canvas_ptr = new std::shared_ptr<SimpleCanvas>(canvas_shared);    
    // Привязываем к окну, чтобы удалить shared_ptr при уничтожении окна
    g_object_set_data_full(G_OBJECT(window), "canvas", canvas_ptr,
        [](gpointer data) {
            delete static_cast<std::shared_ptr<SimpleCanvas>*>(data);
        });
}

void GuiApp::openButtonClick(GtkButton* btn) {
    
}

void GuiApp::saveButtonClick(GtkButton* btn) {

}

void GuiApp::resetButtonClick(GtkButton* btn) {

}

void GuiApp::colorButtonClick(GtkButton* btn) {
    g_print("color button\n");
    GtkColorDialog *dialog = gtk_color_dialog_new();
    gtk_color_dialog_set_title(dialog, "Выберите цвет");
    gtk_color_dialog_choose_rgba(dialog, GTK_WINDOW(window), NULL, NULL, onColorSelected, this);
}

void GuiApp::colorSelect(double red, double green, double blue, double alpha) {
    g_print("Выбран цвет: r=%.2f g=%.2f b=%.2f a=%.2f\n", red, green, blue, alpha);
}

void GuiApp::xSpinnerValueChanged(GtkSpinButton* btn) {

}

void GuiApp::ySpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);    
    g_print("%s: %.0f\n", "ySpinnerValueVhanged", value);

    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(window, "canvas"));
    (*canvas)->rotateAbs((*canvas)->getAngleX(), value, (*canvas)->getAngleZ());
    (*canvas)->redraw();
}

void GuiApp::zSpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "zSpinnerValueVhanged", value);

    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(window, "canvas"));
    (*canvas)->rotateAbs((*canvas)->getAngleX(), (*canvas)->getAngleY(), value);
    (*canvas)->redraw();
}

void GuiApp::shiftSpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "shiftSpinnerValueVhanged", value);
}

void GuiApp::zoomSpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "zoomSpinnerValueVhanged", value);

    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(window, "canvas"));
    (*canvas)->setScale(value);
    (*canvas)->redraw();
}

void GuiApp::lineSwitchActivate(GtkSwitch* sw) {
    gboolean active = gtk_switch_get_active(sw);
    g_print("%d\n", active);
}

void GuiApp::projSwitchActivate(GtkSwitch* sw) {
    gboolean active = gtk_switch_get_active(sw);
    g_print("%d\n", active);
}

void GuiApp::fillSwitchActivate(GtkSwitch* sw) {
    gboolean active = gtk_switch_get_active(sw);
    g_print("%d\n", active);
}

void GuiApp::vertModeToggled(GtkCheckButton* btn, Shape vertMode) {
    g_print("%d\n", vertMode);
}

void GuiApp::openFileSelected(const std::string& path) {
    g_print("%s\n", path.c_str());

    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(window, "canvas"));
    if (canvas) {
        (*canvas)->loadFigure(path.c_str());
        (*canvas)->redraw();
    }
}

void GuiApp::createCommands() {
    this->openCommand = new OpenCommand(this);
    this->saveCommand = new SaveCommand(this);
    this->resetCommand = new ResetCommand(this);
    this->rotateXCommand = new RorateXCommand(this);
    this->shiftCommand = new ShiftCommand(this);
    this->zoomCommand = new ZoomCommand(this);
}

void GuiApp::executeCommand(Command *cmd) {
    if (cmd) {
        cmd->execute();
    }
}

