#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.h"
#include <cmath>
#include <iomanip>


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

	int is_square() {
		if (this->rows == this->cols) return this->rows;
		return 0;
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
		if (this->rix.size() == other.rix.size()) {
			this->rix = other.rix;
		}
	}

	Matrix<T> operator-() const {
		Matrix<T> result(this->cols, this->rows);
		for (int c = 0; c < result.cols; c++) {
			for (int r = 0; r < result.rows; r++) {
				result(r, c) = (*this)(c, r);
			}
		}
		return result;
	}

	Matrix<double> operator*(const double n) const {
		Matrix<double> result(this->rows, this->cols);
		for (size_t i = 0; i < this->rix.size(); i++) {
			result.rix[i] = n * this->rix[i];
		}
		return result;
	}
};

template <typename T>
void printMatrix(const Matrix<T>& mat) {
	std::cout << std::endl << mat.rows << "x" << mat.cols << std::endl;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			std::cout << std::fixed << std::setprecision(2) << mat(i, j) << "  ";
		}
		std::cout << std::endl;
	}
}

template <typename T>
Matrix<T> submatrix(const Matrix<T>& mat, int exclude_row, int exclude_col) {
	Matrix<T> result(mat.rows - 1, mat.cols - 1);
	std::vector<T> temp;
	temp.reserve((mat.rows - 1) * (mat.cols - 1));

	for (int i = 0; i < mat.rows; i++) {
		if (i == exclude_row) continue;

		for (int j = 0; j < mat.cols; j++) {
			if (j == exclude_col) continue;

			temp.push_back(mat(i, j));
		}
	}
	result.rix = std::move(temp);
	return result;
}

template <typename T>
int det(Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");

	if (mat.rows == 2) return mat(0,0) * mat(1,1) - mat(1,0) * mat(0,1);
	int result = 0;
	Matrix<T> tmp(mat.rows - 1, mat.cols - 1);

	for (int i = 0; i < mat.cols; i++) 
	{
		tmp = submatrix(mat, 0, i);
		result += det(tmp) * (std::pow(-1, i) * mat.rix[i]);
	}
	return result;
}

template <typename T>
Matrix<T> adj(Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");

	Matrix<T> result(mat.rows, mat.cols);

	if (mat.rows == 2) {
		result(0, 0) = mat(1, 1); result(0, 1) = -mat(0, 1);
		result(1, 0) = -mat(1, 0); result(1, 1) = mat(0, 0);
		return result;
	}

	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			auto sub = submatrix(mat, i, j);
			result(i, j) = det(sub) * std::pow(-1, i + j);
		}
	}
	return -result;
}

template <typename T>
Matrix<double> inv(Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");

	return adj(mat) * (static_cast<double>(1) / det(mat));
}

template <Streamable T>
void printMatrixAdv(const Matrix<T>& mat) {
	auto biggest_num = std::max_element(mat.rix.begin(), mat.rix.end());
	auto max_digits = width_of(biggest_num);

	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			auto cur = mat(i, j);
			std::cout << cur << std::string(static_cast<size_t>(max_digits - width_of(cur)), ' ') << " ";
		}
		std::cout << std::endl;
	}
}