#include "command.hpp"

#include "gui_app.hpp"

void OpenCommand::execute() {
    if (!app->openDialog->isActive()) {
        app->openDialog->open();    
    }
}

void SaveCommand::execute() {

}

void ResetCommand::execute() {
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs(0, 0, 0);
    (*canvas)->redraw();
}

void RorateXCommand::execute() {
    double value = gtk_spin_button_get_value(app->xSpinnerButton);
    auto* canvas =  static_cast<std::shared_ptr<SimpleCanvas>*>(
        g_object_get_data(app->window, "canvas"));
    (*canvas)->rotateAbs(value, (*canvas)->getAngleY(), (*canvas)->getAngleZ());
    (*canvas)->redraw();
}

void ShiftCommand::execute() {
}

void ZoomCommand::execute() {
}
