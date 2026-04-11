#include "include/globals.h"
namespace s21{

    class Controller{
        protected:
            Figure figure;
        public:
            Controller();
            loadFigure(string filename);
            rotateFigure();
            moveFigure();
            scaleFigure();
    }
}