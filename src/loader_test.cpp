#include "./include/obj_parser/obj_loader.hpp"

int main() {
    char fn[256] = "./assets/teapot.obj";
    s21::ObjLoader loader = s21::ObjLoader();
    loader.loadObjFile(fn);

    int c = 0;
    for (s21::Group_t v : loader.groups) {
        printf("group %d size %ld facets %d\n", c, v.size(), loader.sizeByKind(c, s21::FACE));
        c++;
    }

    return 0;
}
