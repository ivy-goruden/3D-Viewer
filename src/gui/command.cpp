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
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs(app->getAppData().getAngleX(), (*canvas)->getAngleY(), (*canvas)->getAngleZ());
    (*canvas)->redraw();
}

void RotateYCommand::execute() {
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs((*canvas)->getAngleX(), app->getAppData().getAngleY(), (*canvas)->getAngleZ());
    (*canvas)->redraw();
}

void RotateZCommand::execute() {
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs((*canvas)->getAngleX(), (*canvas)->getAngleY(), app->getAppData().getAngleZ());
    (*canvas)->redraw();
}

void ShiftCommand::execute() {
    g_print("%s: %d\n", "shiftSpinnerValueVhanged", app->getAppData().getShift());
}

void ZoomCommand::execute() {
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->setScale(app->getAppData().getZoom());
    (*canvas)->redraw();    
}

void LineSwitchCommand::execute() {
    g_print("%d\n", app->getAppData().getLineSwitch());
}

void ProjSwitchCommand::execute() {
    g_print("%d\n", app->getAppData().getProjSwitch());
}

void FillSwitchCommand::execute() {
    g_print("%d\n", app->getAppData().getFillSwitch());
}

void VertModeCommand::execute() {
    g_print("%d\n", app->getAppData().getVertMode());
}

void ColorCommand::execute() {
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open();    
    }
}
