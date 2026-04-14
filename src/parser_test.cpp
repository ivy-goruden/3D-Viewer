#include "./include/obj_parser/obj_parser.hpp"

int main() {
    char fname[256] = "./assets/cube.obj";
    s21::Obj_Parser parser = s21::Obj_Parser();
    s21::FigureData_t figureData = parser.Load_Figure(fname);

    for (int i = 0; i < figureData.matrix->size(); i++) {
        auto v = figureData.matrix->at(i);
        printf("%5.2f %5.2f %5.2f\n", v[0], v[1], v[2]);
    }

    return 0;
}
