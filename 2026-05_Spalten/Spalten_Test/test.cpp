#include "pch.h"
#include "../Spalten/matrix.h"

TEST(MatrixTests, CanFillAndAccessElements) {
    Matrix<int> mat(2, 3);
    mat.fill_ones();

    EXPECT_EQ(2u, mat.rows);
    EXPECT_EQ(3u, mat.cols);
    EXPECT_EQ(1, mat(0, 0));
    EXPECT_EQ(1, mat(1, 2));
}

TEST(MatrixTests, AdditionAddsElementWise) {
    Matrix<int> left(2, 2, 1);
    Matrix<int> right(2, 2, 2);

    auto result = left + right;

    EXPECT_EQ(3, result(0, 0));
    EXPECT_EQ(3, result(0, 1));
    EXPECT_EQ(3, result(1, 0));
    EXPECT_EQ(3, result(1, 1));
}

TEST(MatrixTests, MultiplyMultipliesMatrices) {
    Matrix<int> left(2, 3);
    Matrix<int> right(3, 2);

    left(0, 0) = 1; left(0, 1) = 2; left(0, 2) = 3;
    left(1, 0) = 4; left(1, 1) = 5; left(1, 2) = 6;

    right(0, 0) = 7; right(0, 1) = 8;
    right(1, 0) = 9; right(1, 1) = 10;
    right(2, 0) = 11; right(2, 1) = 12;

    auto result = left * right;

    EXPECT_EQ(2u, result.rows);
    EXPECT_EQ(2u, result.cols);
    EXPECT_EQ(58, result(0, 0));
    EXPECT_EQ(64, result(0, 1));
    EXPECT_EQ(139, result(1, 0));
    EXPECT_EQ(154, result(1, 1));
}

TEST(MatrixTests, ZeroDeterminantForFlatMatrix) {
    Matrix<int> zeros(3, 3);
    EXPECT_EQ(0, det(zeros));

    Matrix<int> ones(4, 4);
    EXPECT_EQ(0, det(ones));
}

TEST(MatrixTests, IncompatibleDimensiions) {
    Matrix<int> ones(2, 4, 1);
    Matrix<int> twos(3, 3, 2);

    EXPECT_THROW(ones + twos, std::invalid_argument);
    EXPECT_THROW(twos + ones, std::invalid_argument);
    EXPECT_THROW(ones - twos, std::invalid_argument);
    EXPECT_THROW(twos - ones, std::invalid_argument);
    EXPECT_THROW(ones * twos, std::invalid_argument);
    EXPECT_THROW(twos * ones, std::invalid_argument);
    EXPECT_THROW(det(ones), std::invalid_argument);
    EXPECT_THROW(adj(ones), std::invalid_argument);
    EXPECT_THROW(inv(ones), std::invalid_argument);
}

TEST(MatrixTests, DimsEqual) {
    Matrix<int> m(3, 2);
    Matrix<int> b(3, 2);
    Matrix<int> c(2, 1);

    EXPECT_EQ(m.dims_equal(b), true);
    EXPECT_EQ(m.dims_equal(c), false);
    EXPECT_EQ(b.dims_equal(m), true);
    EXPECT_EQ(c.dims_equal(m), false);
}