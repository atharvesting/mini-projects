#pragma once
#include <random>

double generate_random(int low, int high) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(low, high);
	return distr(gen);
}