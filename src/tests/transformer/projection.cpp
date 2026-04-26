// getParallelProjection

#include <gtest/gtest.h>

#include "../../include/main.h"
#include "../../include/globals.h"
#include <limits>

using namespace s21;

TEST(Projection_Test, paralelProjection1) {
  matrix_t shape = {{1,4,0,1}, {4,4,0,1}, {1,1,0,1}, {4,1,0,1}}; 
  Vert_t new_paralel = {Point(1,4), Point(4,4), Point(1,1), Point(4,1)};
  Vert_t project = Transformer::getParallelProjection(shape);

  for (int i=0; i< shape.size(); i++){
        double epsilon = 1e-9; 
        EXPECT_EQ(std::abs(project[i].x - new_paralel[i].x) < epsilon, true);
        EXPECT_EQ(std::abs(project[i].y - new_paralel[i].y) < epsilon, true);
  }
}

TEST(Projection_Test, paralelProjection2) {
  matrix_t shape = {{1,4,5,1}, {4,4,4,1}, {1,1,3,1}, {4,1,5,1}}; 
  Vert_t new_paralel = {Point(1,4), Point(4,4), Point(1,1), Point(4,1)};
  Vert_t project = Transformer::getParallelProjection(shape);

  for (int i=0; i< shape.size(); i++){
        double epsilon = 1e-9; 
        EXPECT_EQ(std::abs(project[i].x - new_paralel[i].x) < epsilon, true);
        EXPECT_EQ(std::abs(project[i].y - new_paralel[i].y) < epsilon, true);
  }

}

TEST(Projection_Test, perspectiveProjection) {
  matrix_t shape = {{0,4,1,1}, {4,4,1,1}, {0,0,1,1}, {4,0,1,1}, {0,4,-1,1}, {4,4,-1,1}, {0,0,-1,1}, {4,0,-1,1}}; 
  Vert_t new_persp = {Point(0,1),Point(1,1),Point(0,0),Point(1,0),Point(0,2),Point(2,2),Point(0,0),Point(2,0)};
  Vert_t project = Transformer::getPerspectiveProjection(shape);

  for (int i=0; i< shape.size(); i++){
        double epsilon = 1e-9; 
        //printf("%f%f - %f%f\n",project[i].x, project[i].y ,new_persp[i].x,new_persp[i].y );
        EXPECT_EQ(std::abs(project[i].x - new_persp[i].x) < epsilon, true);
        EXPECT_EQ(std::abs(project[i].y - new_persp[i].y) < epsilon, true);
  }

}