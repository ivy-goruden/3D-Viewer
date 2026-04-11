#include "../figure/figure.h"
namespace s21{

    typedef std::pair<Poly_t, Vert_t> FigureData_t;

    class Obj_Parser{

        public:
            Obj_Parser();
            FigureData_t Load_Figure(string filename);

    }
}