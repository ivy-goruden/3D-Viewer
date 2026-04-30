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
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setPosX(app->getAppData().getShift());
    canvas->redraw();
}

void ZoomCommand::execute() {
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->setScale(app->getAppData().getZoom());
    (*canvas)->redraw();    
}

void LineSwitchCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setLineType(app->getAppData().getLineSwitch());
    canvas->redraw();
}

void ProjSwitchCommand::execute(){
    SimpleCanvas* canvas = app->getCanvas();
    canvas->toggleProjection();
    canvas->redraw();
}

void FillSwitchCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->togglePolyFill();
    canvas->redraw();
}

void VertModeCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setVertType(app->getAppData().getVertMode());
    canvas->redraw();
}

void ColorCommand::execute() {
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open();    
    }
}
