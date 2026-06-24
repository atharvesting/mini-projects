#pragma once
#include "matrix.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

int main() {
    std::vector<int> vec{ 1, 2, 3, 4 };
    auto v1 = Vector(vec);
    printVec(v1);

    auto v2 = v1 * 2;
    printVec(v2);

    std::cout << v1 * v2 << std::endl;

    return 0;
}
