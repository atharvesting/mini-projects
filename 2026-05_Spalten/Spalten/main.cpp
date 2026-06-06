#pragma once
#include "matrix.h"
#include "utils.h"
#include <iostream>
#include <vector>

int main() {

    Vector<int> v1(std::vector<int>{ 1,0,0 });
    Vector<int> v2(std::vector<int>{ 0,1,0});
    Vector<int> v3(std::vector<int>{ 0,0,1 });

    Matrix<int> m1(3, 3, std::vector<int>{12, -51, 4, 2, 167, -68, -4, 24, -41});
    printMatrix(m1);

    auto qr = qr_decomp(m1);
    printMatrix(qr.first);
    printMatrix(qr.second);

    return 0;

}

//b.fill_random(1, 20);
    //printMatrix(b);
