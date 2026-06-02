#include <gtest/gtest.h>

#include "../../include/main.h"
#include "../../include/globals.h"
#include <limits>

using namespace s21;

static void expectMatrixEq(const matrix_t& a, const matrix_t& b, float eps = 1e-9) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_NEAR(a[i][j], b[i][j], eps);
        }
    }
}


TEST(TransformerScaleTest, UnitScaleKeepsIdentity) {
    Transformer t;
    matrix_t original = {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{0, 0, 0, 1}}
    }};

    matrix_t result = t.Scale(1, 1, 1, original);

    expectMatrixEq(result, original);
}

TEST(TransformerScaleTest, SimpleScale) {
    Transformer t;
    matrix_t original = {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{0, 0, 0, 1}}
    }};

    matrix_t result = t.Scale(2, 3, 4, original);

    matrix_t expected = {{
        {{2, 0, 0, 0}},
        {{0, 3, 0, 0}},
        {{0, 0, 4, 0}},
        {{0, 0, 0, 1}}
    }};

    expectMatrixEq(result, expected);
}

TEST(TransformerScaleTest, ScaleWithNonIdentityOriginal) {
    Transformer t;
    matrix_t original = {{
        {{2, 0, 0, 0}},
        {{0, 2, 0, 0}},
        {{0, 0, 2, 0}},
        {{0, 0, 0, 1}}
    }};

    matrix_t result = t.Scale(3, 4, 5, original);

    matrix_t scale_m = {{
    {{3, 0, 0, 0}},
    {{0, 4, 0, 0}},
    {{0, 0, 5, 0}},
    {{0, 0, 0, 1}}
    }};

    matrix_t expected = Matrix::multiplyMatrix(&original, &scale_m);

    expectMatrixEq(result, expected);
}
