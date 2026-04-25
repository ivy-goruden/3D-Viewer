#include "gui_app.hpp"
#include <gtk/gtk.h>
#include <iostream>

void GuiApp::onActivate(GtkApplication *app, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->activate(app);
}

void GuiApp::onOpenButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->openButtonClick(btn);
}

void GuiApp::onSaveButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->saveButtonClick(btn);
}

void GuiApp::onResetButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->resetButtonClick(btn);
}

void GuiApp::onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->xSpinnerValueChanged(btn);
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

GuiApp::GuiApp() {
    app = gtk_application_new("com.application", G_APPLICATION_DEFAULT_FLAGS);
}

GuiApp::~GuiApp() {
    g_object_unref(app);
}

int GuiApp::run(int argc, char **argv) {
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), this);
    return g_application_run(G_APPLICATION(app), argc, argv);    return status;

}

void GuiApp::activate(GtkApplication* app) {
    GtkBuilder* builder = gtk_builder_new_from_file("gui/3dviewer.ui");
    window = gtk_builder_get_object(builder, "main");
    openButton = gtk_builder_get_object(builder, "open_button");
    saveButton = gtk_builder_get_object(builder, "save_button");
    resetButton = gtk_builder_get_object(builder, "reset_button");
    xSpinnerButton = gtk_builder_get_object(builder, "rotate_x_spinner");
    ySpinnerButton = gtk_builder_get_object(builder, "rotate_y_spinner");
    zSpinnerButton = gtk_builder_get_object(builder, "rotate_z_spinner");
    shiftSpinnerButton = gtk_builder_get_object(builder, "shift_spinner");
    zoomSpinnerButton = gtk_builder_get_object(builder, "zoom_spinner");
    g_object_unref(builder);

    g_signal_connect(openButton, "clicked", G_CALLBACK(onOpenButtonClick), this);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(onSaveButtonClick), this);
    g_signal_connect(resetButton, "clicked", G_CALLBACK(onResetButtonClick), this);
    g_signal_connect(xSpinnerButton, "value_changed", G_CALLBACK(onXSpinnerValueChanged), this);
    g_signal_connect(ySpinnerButton, "value_changed", G_CALLBACK(onYSpinnerValueChanged), this);
    g_signal_connect(zSpinnerButton, "value_changed", G_CALLBACK(onZSpinnerValueChanged), this);
    g_signal_connect(shiftSpinnerButton, "value_changed", G_CALLBACK(onShiftSpinnerValueChanged), this);
    g_signal_connect(zoomSpinnerButton, "value_changed", G_CALLBACK(onZoomSpinnerValueChanged), this);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));
}

void GuiApp::openButtonClick(GtkButton* btn) {
    g_print("open button\n");
}

void GuiApp::saveButtonClick(GtkButton* btn) {
    g_print("save button\n");
}

void GuiApp::resetButtonClick(GtkButton* btn) {
    g_print("reset button\n");
}

void GuiApp::xSpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "xSpinnerValueVhanged", value);
}

void GuiApp::ySpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "ySpinnerValueVhanged", value);
}

void GuiApp::zSpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "zSpinnerValueVhanged", value);
}

void GuiApp::shiftSpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "shiftSpinnerValueVhanged", value);
}

void GuiApp::zoomSpinnerValueChanged(GtkSpinButton* btn) {
    double value = gtk_spin_button_get_value(btn);
    g_print("%s: %.0f\n", "zoomSpinnerValueVhanged", value);
}
