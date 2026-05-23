#include "model.h"
#include "utils.h"
#include <iostream>

int main() {

	Matrix<int> m(4, 4);

	m.fill_random(1, 9);
	printMatrix(m);

	auto b = adj(m);
	printMatrix(b);

	auto i = inv(m);
	printMatrix(i);

	//std::cout << std::endl << adj(m) << std::endl;

	//auto c = submatrix(m, 0, 1);
	//printMatrix(c);

	return 0;

}

//b.fill_random(1, 20);
	//printMatrix(b);