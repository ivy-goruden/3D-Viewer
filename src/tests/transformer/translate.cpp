#include <gtest/gtest.h>

#include "../../include/main.h"
#include "../../include/globals.h"
#include <limits>

using namespace s21;

static void expectMatrixEq(const matrix_t& a, const matrix_t& b, double eps = 1e-9) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_NEAR(a[i][j], b[i][j], eps);
        }
    }
}

TEST(TransformerTranslateTest, ZeroTranslationKeepsIdentity) {
    Transformer t;
    matrix_t original = {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{0, 0, 0, 1}}
    }};

    matrix_t result = t.Translate(0, 0, 0, original);

    matrix_t expected = original;
    expectMatrixEq(result, expected);
}

TEST(TransformerTranslateTest, SimpleTranslation) {
    Transformer t;
    matrix_t original = {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{0, 0, 0, 1}}
    }};

    matrix_t result = t.Translate(2, 3, 4, original);

    matrix_t expected = {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{2, 3, 4, 1}}
    }};

    expectMatrixEq(result, expected);
}

TEST(TransformerTranslateTest, TranslationWithNonIdentityOriginal) {
    Transformer t;
    matrix_t original = {{
        {{2, 0, 0, 0}},
        {{0, 2, 0, 0}},
        {{0, 0, 2, 0}},
        {{1, 1, 1, 1}}
    }};

    matrix_t result = t.Translate(1, 2, 3, original);

    matrix_t translate_m = {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{1, 2, 3, 1}}
    }};

    matrix_t expected = Matrix::multiplyMatrix(&original, &translate_m);

    expectMatrixEq(result, expected);
}