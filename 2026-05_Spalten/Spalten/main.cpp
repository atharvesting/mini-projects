#pragma once
#include "matrix.h"
#include "utils.h"
#include <iostream>

int main() {

    Vector<int> v1(std::vector<int>{ 1,0,0 });
    Vector<int> v2(std::vector<int>{ 0,1,0});
    Vector<int> v3(std::vector<int>{ 0,0,1 });

    Matrix<int> m1(3, 3, std::vector<int>{1, 0, 0, 0, 1, 0, 0, 0, 1});
    printMatrix(m1);
    
    auto Vs = mat_to_vec(m1);

    for (const auto& vec : Vs) {
        printVec(vec);
    }

    return 0;

}

//b.fill_random(1, 20);
    //printMatrix(b);
