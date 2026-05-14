#include "model.h"
#include <iostream>

int main() {

	Matrix<int> m(3, 4), b(3, 4);

	m.fill_random(1, 5);
	printMatrix(m);

	b.fill_random(1, 5);
	printMatrix(b);

	auto c = m - b;
	printMatrix(c);

	return 0;

}