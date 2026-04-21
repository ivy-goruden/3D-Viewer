#include "./include/obj_parser/obj_loader.hpp"

int main() {
    char fname[256] = "./assets/cube.obj";
    s21::ObjLoader loader = s21::ObjLoader();
    loader.loadObjFile(fname);

    for (s21::VertexObj_t v : loader.vertices) {
        printf("%0.2f, %0.2f, %0.2f\n", v.x, v.y, v.z);
    }

    return 0;
}
