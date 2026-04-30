#include <gtest/gtest.h>

#include "../../include/main.h"
#include "../../include/globals.h"

using namespace s21;
// Test case 1
TEST(Pyramid_Test, vertices) {
    char fname[256] = "../../assets/pyramid.obj";
    s21::ObjLoader loader = s21::ObjLoader();
    loader.loadObjFile(fname);

    EXPECT_EQ(loader.vertices.size(), 5);
}

TEST(Pyramid_Test, lines) {
    char fname[256] = "../../assets/pyramid.obj";
    s21::ObjLoader loader = s21::ObjLoader();
    loader.loadObjFile(fname);

    EXPECT_EQ(loader.lines.size(), 0);
}

TEST(Pyramid_Test, faces) {
    char fname[256] = "../../assets/pyramid.obj";
    s21::ObjLoader loader = s21::ObjLoader();
    loader.loadObjFile(fname);

    EXPECT_EQ(loader.faces.size(), 6);
}

