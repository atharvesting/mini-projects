#include "model.h"
#include <iostream>

int main() {

	Matrix<int> m(3, 4);

	m.fill_zeros();
	printMatrix(m);

	m.fill_ones();
	printMatrix(m);

	m.fill_random(1, 20);
	printMatrix(m);

	std::cout << "Copied\n" << std::endl;
	Matrix<int> b(4, 3);
	b.copy(m);
	printMatrix(b);
	printMatrix(m);

	//auto c = m + b;
	//printMatrix(c);

	return 0;

}