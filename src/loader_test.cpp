#include "./include/obj_parser/obj_loader.hpp"

int main() {
    char fname[256] = "./assets/cube.obj";
    s21::ObjLoader loader = s21::ObjLoader();
    loader.loadObjFile(fname);
    return 0;
}