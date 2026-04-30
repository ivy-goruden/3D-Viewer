#include <gtest/gtest.h>

#include "../../include/main.h"
#include "../../include/globals.h"
#include <limits>

using namespace s21;
// Test case 1
TEST(Rotate_Test, x_rotate) {
  matrix_t shape = {{1,4,0,1}, {4,4,0,1}, {1,1,0,1}, {4,1,0,1}}; 
  matrix_t shape_new = {{1,0,0,1}, {4,0,0,1}, {1,0,0,1}, {4,0,0,1}}; 
  matrix_t project = Transformer::Rotate(90,0,0,shape);

  for (int i=0; i< shape.size(); i++){
    for (int x=0; x< 2; x++){
        double epsilon = 1e-9; 
        EXPECT_EQ(std::abs(project[i][x] - shape_new[i][x]) < epsilon, true);
    }
  }
}

TEST(Rotate_Test, y_rotate) {
  matrix_t shape = {{1,4,0,1}, {4,4,0,1}, {1,1,0,1}, {4,1,0,1}}; 
  matrix_t shape_new = {{0,4,0,1}, {0,4,0,1}, {0,1,0,1}, {0,1,0,1}}; 
  matrix_t project = Transformer::Rotate(0,90,0,shape);

  for (int i=0; i< shape.size(); i++){
    for (int x=0; x< 2; x++){
        double epsilon = 1e-9; 
        EXPECT_EQ(std::abs(project[i][x] - shape_new[i][x]) < epsilon, true);
    }
  }

}

TEST(Rotate_Test, z_rotate) {
  matrix_t shape = {{1,4,1,1}, {4,4,3,1}, {1,1,1,1}, {4,1,3,1}}; 
  matrix_t shape_new = {{4,-1,0,1}, {4,-4,0,1}, {1,-1,0,1}, {1,-4,0,1}}; 
  matrix_t project = Transformer::Rotate(0,0,90,shape);

  for (int i=0; i< shape.size(); i++){
    for (int x=0; x< 2; x++){
        double epsilon = 1e-9; 
        EXPECT_EQ(std::abs(project[i][x] - shape_new[i][x]) < epsilon, true);
    }
  }

}

