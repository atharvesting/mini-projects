#pragma once
#include <random>
#include <cmath>

double generate_random(int low, int high) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(low, high);
	return distr(gen);
}

int countDigits(int n) {
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10; // for 32-bit max
}

template <typename T>
int no_of_digits(T num) {
    int int_part = std::floor(num);
    double deci_part = num - int_part;
    std::cout << deci_part << std::endl;
    int i = 0;
    while (std::floor(deci_part) - deci_part < 0 && i < 5) {
        deci_part *= 10;
        i++;
    }
    std::cout << countDigits(int_part) << "   " << i << std::endl;
    return countDigits(int_part) + countDigits(deci_part);
}