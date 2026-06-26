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
    s21::Controller *c_ = app->getController();
    c_->setAngleX(0);
    c_->setAngleY(0);
    c_->setAngleZ(0);
    c_->shiftX(0);
    c_->shiftY(0);
}

void RotateXCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->setAngleX(app->getAppData()->getAngleX());
}

void RotateYCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->setAngleY(app->getAppData()->getAngleY());
}

void RotateZCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->setAngleZ(app->getAppData()->getAngleZ());
}

void ShiftCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->shiftX(app->getAppData()->getShift());
}

void ShiftVCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->shiftY(app->getAppData()->getShiftV());
}

void ZoomCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->setCameraZ(app->getAppData()->getZoom());
}

void ScaleCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->setScale(app->getAppData()->getScale());
}

void LineSwitchCommand::execute() {
    SimpleCanvas *canvas = app->getCanvas();
    canvas->setLineType(app->getAppData()->getLineSwitch());
}

void ProjSwitchCommand::execute() {
    s21::Controller *c_ = app->getController();
    c_->toggleProjection();
}

void FillSwitchCommand::execute() {
    SimpleCanvas *canvas = app->getCanvas();
    canvas->togglePolyFill();
}

void VertModeCommand::execute() {
    SimpleCanvas *canvas = app->getCanvas();
    canvas->setVertType(app->getAppData()->getVertMode());
}

void DotColorCommand::execute() {
    app->colorDialog->setOnColorSelectedCallback(
        [this](float red, float green, float blue, float alpha) { app->dotColorSelected(red, green, blue, alpha); });
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open(app->getAppData()->getDotColor());
    }
}

void BgColorCommand::execute() {
    app->colorDialog->setOnColorSelectedCallback(
        [this](float red, float green, float blue, float alpha) { app->bgcolorSelected(red, green, blue, alpha); });
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open(app->getAppData()->getBgColor());
    }
}

void PolyColorCommand::execute() {
    app->colorDialog->setOnColorSelectedCallback(
        [this](float red, float green, float blue, float alpha) { app->polyColorSelected(red, green, blue, alpha); });
    if (!app->colorDialog->isActive()) {
        app->colorDialog->open(app->getAppData()->getPolyColor());
    }
}


void WeightCommand::execute() {
    SimpleCanvas *canvas = app->getCanvas();
    canvas->setLineWidth(app->getAppData()->getWeight());
}

void VertSizeCommand::execute() {
    SimpleCanvas *canvas = app->getCanvas();
    canvas->setVertSize(app->getAppData()->getVertSize());
}
