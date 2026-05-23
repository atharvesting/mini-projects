#pragma once
#include <random>
#include <cmath>
#include <sstream>

double generate_random(int low, int high) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(low, high);
	return distr(gen);
}

template <typename T>
concept Streamable =
	requires(std::ostream & os, T a) {
		os << a;
	};

template <Streamable T>
requires std::is_arithmetic_v<T>
int width_of(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str().length();
}

//int countDigits(int n) {
//    if (n < 10) return 1;
//    if (n < 100) return 2;
//    if (n < 1000) return 3;
//    if (n < 10000) return 4;
//    if (n < 100000) return 5;
//    if (n < 1000000) return 6;
//    if (n < 10000000) return 7;
//    if (n < 100000000) return 8;
//    if (n < 1000000000) return 9;
//    return 10; // for 32-bit max
//}
//
//template <typename T>
//int no_of_digits(T num) {
//    int int_part = std::floor(static_cast<double>(num));
//    int deci_part = static_cast<int>(std::round((num - int_part) * 100000));
//
//    while (static_cast<int>(deci_part) % 10 == 0) deci_part = deci_part / 10;
//
//    return countDigits(int_part) + countDigits(static_cast<int>(deci_part));
//}