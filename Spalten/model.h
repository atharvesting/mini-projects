#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.h"

template <typename T>
class Matrix {
public:
	size_t rows;
	size_t cols;
	std::vector<T> rix;

	Matrix(int my_dim_r, int my_dim_c)
		: rows(my_dim_r), cols(my_dim_c), rix(my_dim_r * my_dim_c, 0) 
	{
		if (my_dim_r < 0 or my_dim_c < 0) {
			throw std::invalid_argument("Matrix dimensions must be non-negative.");
		}
	}

	void fill_zeros() {
		std::fill(rix.begin(), rix.end(), static_cast<T>(0));
	}

	// Fill matrix with random values. High and low inclusive.
	void fill_random(int low, int high) {
		for (auto& num : rix) {
			num = static_cast<T>(generate_random(low, high));
		}
	}

	void fill_ones() {
		std::fill(rix.begin(), rix.end(), static_cast<T>(1));
	}

	const T& operator()(size_t r, size_t c) const {
		return rix[r * cols + c];
	}

	T& operator()(size_t r, size_t c) {
		return rix[r * cols + c];
	}

	Matrix<T> operator+(const Matrix<T>& other) const {
		if (this->rows == other.rows and this->cols == other.cols) {
			Matrix<T> result(rows, cols);
			for (size_t i = 0; i < rix.size(); i++)
				result.rix[i] = rix[i] + other.rix[i];
			return result;
		}
		else {
			throw std::invalid_argument("Dimensions of matrices must match for addition.");
		}
	}

	Matrix<T> operator-(const Matrix<T>& other) const {
		if (this->rows == other.rows and this->cols == other.cols) {
			Matrix<T> result(rows, cols);
			for (size_t i = 0; i < rix.size(); i++)
				result.rix[i] = rix[i] - other.rix[i];
			return result;
		}
		else {
			throw std::invalid_argument("Dimensions of matrices must match for subtraction.");
		}
	}

	Matrix<T> operator*(const Matrix<T>& other) const {
		if (this->cols == other.rows) {
			Matrix<T> result(this->rows, other.cols);
			for (int i = 0; i < this->rows; i++) {
				for (int j = 0; j < other.cols; j++) {
					for (int k = 0; k < this->cols; k++) {
						result(i, j) += (*this)(i, k) * other(k, j);
					}
				}
			}
			return result;
		}
		else {
			throw std::invalid_argument("Column count of first and Row count of second must match for matrix multiplication.");
		}
	}

	void copy(Matrix<T>& other) {
		if (rix.size() == other.rix.size()) {
			rix = other.rix;
		}
	}

};

template <typename T>
void printMatrix(const Matrix<T>& mat) {
	std::cout << std::endl << mat.rows << "x" << mat.cols << std::endl;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			std::cout << mat(i, j) << "  ";
		}
		std::cout << std::endl;
	}
}