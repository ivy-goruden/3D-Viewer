#ifndef OBJ_PARSER
#define OBJ_PARSER
#include "../globals.h"
#include <string>
namespace s21{

    class Obj_Parser{

        public:
            Obj_Parser();
            FigureData_t Load_Figure(std::string filename);

    };
}
#endif