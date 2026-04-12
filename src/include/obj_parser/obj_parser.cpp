#include "obj_parser.hpp"

namespace s21{
    Obj_Parser::Obj_Parser(){}
    FigureData_t Obj_Parser::Load_Figure(std::string filename){
        matrix_t m = {{0.0}};
        Poly_t p = {{0}};
        FigureData_t data = {&m, &p};
        return data;
    }
}