#include "gui.hpp"
#include "../gui/application_path.hpp"
#include "../include/globals.h"
#include "../include/affine_transformer/matrix.hpp"
#include "../include/affine_transformer/transformer.hpp"
#include <format>
#include <string>
#include <iostream>

int main (int argc, char **argv) {
    ApplicationPath app_paths(argv[0]);
    GuiApp app;
    app.ui_file = app_paths.get_path_relative_to_exe("3dviewer.ui").string();
    return app.run(argc, argv);
}
