#pragma once
#include "matrix.h"
#include "utils.h"
#include <iostream>

int main() {
    Matrix<int> m(2, 4);
    m.fill_random_int(1, 9);
    printMatrix(m);

    std::set<int> rows{};
    std::set<int> cols{ 1, 3 };

    auto b = submatrix(m, rows, cols);
    printMatrix(b);

    //auto c = submatrix(m, 0, 1);
    //printMatrix(c);

    return 0;

}

//b.fill_random(1, 20);
    //printMatrix(b);
