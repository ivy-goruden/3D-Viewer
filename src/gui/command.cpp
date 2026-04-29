#include "command.hpp"

#include "gui_app.hpp"

void OpenCommand::execute() {
    if (!app->openDialog->isActive()) {
        app->openDialog->open();    
    }
}

void SaveCommand::execute() {
    g_print("%s\n", "save command");
}

void ResetCommand::execute() {
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs(0, 0, 0);
    (*canvas)->redraw();
}

void RotateXCommand::execute() {
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->xSpinnerButton));
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs(value, (*canvas)->getAngleY(), (*canvas)->getAngleZ());
    (*canvas)->redraw();
}

void RotateYCommand::execute() {
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->ySpinnerButton));
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs((*canvas)->getAngleX(), value, (*canvas)->getAngleZ());
    (*canvas)->redraw();
}

void RotateZCommand::execute() {
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->zSpinnerButton));
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs((*canvas)->getAngleX(), (*canvas)->getAngleY(), value);
    (*canvas)->redraw();
}

void ShiftCommand::execute() {
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->shiftSpinnerButton));
    g_print("%s: %.0f\n", "shiftSpinnerValueVhanged", value);
}

void ZoomCommand::execute() {
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->zoomSpinnerButton));
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->setScale(value);
    (*canvas)->redraw();    
}

void LineSwitchCommand::execute() {
    bool active = gtk_switch_get_active(GTK_SWITCH(app->lineSwitch));
    g_print("%d\n", active);

}

void ProjSwitchCommand::execute() {
    bool active = gtk_switch_get_active(GTK_SWITCH(app->projSwitch));
    g_print("%d\n", active);

}

void FillSwitchCommand::execute() {
    bool active = gtk_switch_get_active(GTK_SWITCH(app->fillSwitch));
    g_print("%d\n", active);
}

void VertModeCommand::execute() {
    g_print("%d\n", app->getAppData().getVertMode());
}

void ColorCommand::execute() {
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open();    
    }
}
