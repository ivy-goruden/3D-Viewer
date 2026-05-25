#include "command.hpp"

#include <format>

#include "app_data.hpp"
#include "gui_app.hpp"

void OpenCommand::execute() {
    if (!app->openDialog->isActive()) {
        app->openDialog->open();
    }
}

void ResetCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(0, 0, 0);
    canvas->shiftX(0);
    canvas->shiftY(0);
    canvas->redraw();
}

void RotateXCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(app->getAppData()->getAngleX(), canvas->getAngleY(), canvas->getAngleZ());
    canvas->redraw();
}

void RotateYCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(canvas->getAngleX(), app->getAppData()->getAngleY(), canvas->getAngleZ());
    canvas->redraw();
}

void RotateZCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->rotateAbs(canvas->getAngleX(), canvas->getAngleY(), app->getAppData()->getAngleZ());
    canvas->redraw();
}

void ShiftCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->shiftX(app->getAppData()->getShift());
    canvas->redraw();
}

void ShiftVCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->shiftY(app->getAppData()->getShiftV());
    canvas->redraw();
}

void ZoomCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setCamera(app->getAppData()->getZoom());
    canvas->redraw();
}

void ScaleCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setScale(app->getAppData()->getScale());
    canvas->redraw();
}

void LineSwitchCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setLineType(app->getAppData()->getLineSwitch());
    canvas->redraw();
}

void ProjSwitchCommand::execute() {
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
    canvas->setVertType(app->getAppData()->getVertMode());
    canvas->redraw();
}

void ColorCommand::execute() {
    app->colorDialog->setOnColorSelectedCallback(
        [this](double red, double green, double blue, double alpha) { app->colorSelected(red, green, blue, alpha); });
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open(app->getAppData()->getColor());
    }
}

void BgColorCommand::execute() {
    app->colorDialog->setOnColorSelectedCallback(
        [this](double red, double green, double blue, double alpha) { app->bgcolorSelected(red, green, blue, alpha); });
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open(app->getAppData()->getBgColor());
    }
}

void WeightCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setLineWidth(app->getAppData()->getWeight());
    canvas->redraw();
}

void VertSizeCommand::execute() {
    SimpleCanvas* canvas = app->getCanvas();
    canvas->setVertSize(app->getAppData()->getVertSize());
    canvas->redraw();
}
