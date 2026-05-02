#include "obj_parser.hpp"

#include "obj_loader.hpp"
#include <memory>

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
            std::vector<double> vc = {v.x, v.y, v.z};
            data.matrix->push_back(vc);
        }
        printf("groups %ld\n", loader.groups.size());
        if (loader.groups.size() > 0) {
            for (s21::GroupElement_t e : loader.groups.at(0)) {
                printf("group size %ld; kind: %d\n", loader.groups.at(0).size(), e.kind);
                if (e.kind == FACE) {
                    std::vector<int> poli;
                    for (s21::FaceElementObj_t fe : loader.faces.at(e.index)) {
                        poli.push_back(fe.vi);
                    }
                    data.polygons->push_back(poli);
                }
            }
        } else {
            for (s21::FaceObj_t f : loader.faces) {
                std::vector<int> poli;
                for (s21::FaceElementObj_t fe : f) {
                    poli.push_back(fe.vi);
                }
                data.polygons->push_back(poli);
            }
        }

        return data;
    }

}
