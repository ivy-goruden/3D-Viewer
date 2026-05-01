#include "gui/gui_app.hpp"
#include "gui/application_path.hpp"
  
int main (int argc, char **argv) {
    ApplicationPath app_paths(argv[0]);

    GuiApp app;
    app.ui_file = app_paths.get_path_relative_to_exe("gui/3dviewer.ui").string();
    return app.run(argc, argv);
}
