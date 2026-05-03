#include "command.hpp"

#include "gui_app.hpp"
#include "app_data.hpp"

void OpenCommand::execute() {
    if (!app->openDialog->isActive()) {
        app->openDialog->open();    
    }
}

void SaveCommand::execute() {
    g_print("%s\n", "save command");
}

void ResetCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(0, 0, 0);
    canvas->redraw();
}

void RotateXCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(app->getAppData().getAngleX(), canvas->getAngleY(), canvas->getAngleZ());
    canvas->redraw();
}

void RotateYCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(canvas->getAngleX(), app->getAppData().getAngleY(), canvas->getAngleZ());
    canvas->redraw();
}

void RotateZCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(canvas->getAngleX(), canvas->getAngleY(), app->getAppData().getAngleZ());
    canvas->redraw();   
}

void ShiftCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setPosX(app->getAppData().getShift());
    canvas->redraw();
}

void ZoomCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setZoom(double(app->getAppData().getZoom())/100);
    canvas->redraw();
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
    app->colorDialog->setOnColorSelectedCallback(
        [this](double red, double green, double blue, double alpha) {
            app->colorSelected(red, green, blue, alpha);
        }
    );
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open(app->getAppData().getColor());
    }
}

void BgColorCommand::execute() {
    app->colorDialog->setOnColorSelectedCallback(
        [this](double red, double green, double blue, double alpha) {
            app->bgcolorSelected(red, green, blue, alpha);
        }
    );
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open(app->getAppData().getBgColor());
    }
}

void WeightCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setLineWidth(app->getAppData().getWeight()*0.01);
    canvas->redraw();
}

