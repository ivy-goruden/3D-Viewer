#include <gtest/gtest.h>

#include "../../include/main.h"
#include "../../include/globals.h"
#include <limits>

using namespace s21;

TEST(Projection_Test, paralelProjection1) {
  matrix_t shape = {{1,4,0,1}, {4,4,0,1}, {1,1,0,1}, {4,1,0,1}}; 
  Vert_t new_paralel = {Point(1,4), Point(4,4), Point(1,1), Point(4,1)};
  Vert_t project = Transformer::getParallelProjection(shape, 1);

  for (int i=0; i< shape.size(); i++){
        double epsilon = 1e-9; 
        EXPECT_EQ(std::abs(project[i].x - new_paralel[i].x) < epsilon, true);
        EXPECT_EQ(std::abs(project[i].y - new_paralel[i].y) < epsilon, true);
  }
}

TEST(Projection_Test, paralelProjection2) {
  matrix_t shape = {{1,4,5,1}, {4,4,4,1}, {1,1,3,1}, {4,1,5,1}}; 
  Vert_t new_paralel = {Point(1,4), Point(4,4), Point(1,1), Point(4,1)};
  Vert_t project = Transformer::getParallelProjection(shape,1);

  for (int i=0; i< shape.size(); i++){
        double epsilon = 1e-9; 
        EXPECT_EQ(std::abs(project[i].x - new_paralel[i].x) < epsilon, true);
        EXPECT_EQ(std::abs(project[i].y - new_paralel[i].y) < epsilon, true);
  }

}

static void expectPointNear(const s21::Point& a, const s21::Point& b, double eps = 1e-9) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
}

TEST(TransformerPerspectiveTest, ProjectsSinglePoint) {
    Transformer t;

    matrix_t m = {{
        {{2, 4, 8, 1}}
    }};

    double camera = 2.0;
    double scale = 10.0;
    double minz = 0.0;

    Vert_t result = t.getPerspectiveProjection(m, camera, scale, minz);

    ASSERT_EQ(result.size(), 1u);
    expectPointNear(result[0], s21::Point{2.0, 4.0});
}

TEST(TransformerPerspectiveTest, SkipsPointBehindMinZ) {
    Transformer t;

    matrix_t m = {{
        {{1, 1, -4, 1}}
    }};

    double camera = 0.0;
    double scale = 10.0;
    double minz = 5.0;

    Vert_t result = t.getPerspectiveProjection(m, camera, scale, minz);

    ASSERT_EQ(result.size(), 0u);
}