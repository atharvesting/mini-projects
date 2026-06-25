#pragma once
#include "matrix.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

int main() {
    
    auto m1 = mat_random_int_range(4, 4, 1, 9);
    printMatrix(m1);
    
    auto lu = LU_decomp(m1);
    lu.print();

    return 0;
}
