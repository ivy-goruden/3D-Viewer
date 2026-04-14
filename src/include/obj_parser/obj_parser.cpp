#include "obj_parser.hpp"

#include "obj_loader.hpp"

namespace s21{

    Obj_Parser::Obj_Parser() {}

    FigureData_t Obj_Parser::Load_Figure(std::string filename){
        auto m = std::make_unique<matrix_t>();
        auto p = std::make_unique<Poly_t>();
        FigureData_t data = {std::move(m), std::move(p)};

        const char* fn = filename.c_str();
        s21::ObjLoader loader = s21::ObjLoader();
        loader.loadObjFile(fn);

        for (s21::VertexObj_t v : loader.vertices) {
            printf("%0.2f, %0.2f, %0.2f\n", v.x, v.y, v.z);
            std::vector<double> vc = {v.x, v.y, v.z};
            data.matrix->push_back(vc);
        }

        return data;
    }

}
