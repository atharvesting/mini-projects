#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.h"
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <set>
#include <numeric>
#include <execution>
#include <functional>

template <typename T>
class Vector {
public:
	std::vector<T> vec;
	float mag;
	int dim;

	Vector(std::vector<T> rray)
		: vec(rray), mag(magnitude(vec)), dim(vec.size()) {
	}

	Vector(std::vector<T>&& rray) 
		: vec(std::move(rray)), mag(magnitude(vec)), dim(vec.size()) {}

	Vector(int dim)
		: dim(dim), mag(0.0f), vec(dim, T{ 0 }) { }

	using iterator = std::vector<T>::iterator;
	using const_iterator = std::vector<T>::const_iterator;

	iterator begin() { return vec.begin(); }
	iterator end() { return vec.end(); }

	const_iterator begin() const { return vec.begin(); }
	const_iterator end() const { return vec.end(); }

	static float magnitude(const std::vector<T>& arr) {
		auto square_sum = std::reduce(std::execution::par_unseq,
			arr.begin(),
			arr.end(),
			T{ 0 },
			[](T total, T num) { return total + num * num; }
		);
		return static_cast<float>(std::sqrt(square_sum));
	}

	static float magnitude(const Vector<T>& vector) {
		return magnitude(vector.vec);
	}

	bool dims_match(const Vector& other) const {
		return this->dim == other.dim;
	}

	const T operator()(size_t idx) const {
		return vec[idx];
	}

	T operator()(size_t idx) {
		return vec[idx];
	}

	Vector<T> element_wise(const Vector& other, std::function<T(T, T)> func) const {
		if (!dims_match(other)) throw std::invalid_argument("Vector dimensions must match.");
		Vector<T> result(this->dim);
		std::transform(vec.begin(), vec.end(), other.begin(), result.begin(), func);
		return result;
	}

	Vector<T> operator+(const Vector& other) const {
		return element_wise(other, std::plus<T>());
	}

	Vector<T> operator-(const Vector& other) const {
		return element_wise(other, std::minus<T>());
	}

	T operator*(const Vector& other) const {
		if (!dims_match(other)) throw std::invalid_argument("Vector dimensions must match.");
		return std::inner_product(vec.begin(), vec.end(), other.begin(), 0);
	}

	Vector<T> operator*(const float scalar) const {
		Vector<T> result(this->dim);
		std::transform(std::execution::par,
			vec.begin(),
			vec.end(),
			result.begin(), [scalar](T val) { return val * scalar; }
		);
		return result;
	}

	template <typename U>
	Vector<float> operator/(U scalar) const {
		if (scalar == 0) throw std::invalid_argument("Divisor must not be Zero.");
		Vector<float> result(this->dim);
		std::transform(std::execution::par, 
			vec.begin(), 
			vec.end(), 
			result.begin(), 
			[scalar](T val) { 
				return static_cast<float>(val) / static_cast<float>(scalar); 
			}
		);
		return result;
	}

	Vector<float> normalize() const {
		if (magnitude((*this)) == 0) throw std::invalid_argument("Cannot normalize a Zero-Vector.");
		return (*this) / magnitude(*this);
	}

	float scalar_proj_on(const Vector& other) {
		return (*this * other) / magnitude(other);
	}

	Vector<float> vector_proj_on(const Vector& other) {
		return other.normalize() * scalar_proj_on(other);
	}

	float angle_between(const Vector& other) {
		return std::acos(((*this) * other) / (magnitude(*this) * magnitude(other)));
	}

	float cosine_similarity(const Vector& other) {
		return ((*this) * other) / (magnitude(*this) * magnitude(other));
	}

	Vector<float> to_float() const {
		std::vector<float> converted(vec.begin(), vec.end());
		return Vector<float>(std::move(converted));
	}

	template <typename... Args>
	static std::vector<Vector<float>> gram_schmidt(Args&&... args) 
	{
		std::vector<Vector<T>> input_vectors{ std::forward<Args>(args)... };
		std::vector<Vector<float>> ortho_basis;
		if (input_vectors.empty()) return ortho_basis;

		ortho_basis.push_back(input_vectors[0].normalize());

		for (size_t i = 1; i < input_vectors.size(); i++) 
		{
			Vector<float> v = input_vectors[i].to_float();
			for (const auto& basis: ortho_basis) 
			{
				Vector<float> projection = input_vectors[i].to_float().vector_proj_on(basis);
				v = v - projection;
			}
			ortho_basis.push_back(v.normalize());
		}
		return ortho_basis;
	}
};

template <typename T>
void printVec(const Vector<T> vector) {
	for (size_t i = 0; i < vector.dim; i++) {
		std::cout << vector(i) << "\n";
	}
	std::cout << std::endl;
}


template <typename T>
class Matrix {
public:
	size_t rows;
	size_t cols;
	std::vector<T> rix;

	Matrix(size_t r, size_t c)
		: rows(r), cols(c), rix(r* c, T{}) {
	}

	Matrix(size_t dim)
		: rows(dim), cols(dim), rix(dim* dim, T{}) {
	}

	Matrix(size_t r, size_t c, const T& fill)
		: rows(r), cols(c), rix(r* c, fill) {
	}

	Matrix(size_t r, size_t c, std::vector<T>&& nums)
		: rows(std::move(r)), cols(std::move(c)) {
		if (nums.size == rows * cols) {
			rix = std::move(nums);
		}
	}

	// compiler confuses this constructor with the first one
	//Matrix(size_t dim, const T& fill)
	//	: rows(dim), cols(dim), rix(dim * dim, fill) {
	//}

	int is_square() {
		if (this->rows == this->cols) return this->rows;
		return 0;
	}

	void fill_zeros() {
		std::fill(rix.begin(), rix.end(), static_cast<T>(0));
	}

	// Fill matrix with random values. High and low inclusive.
	void fill_random_int(int low, int high) {
		for (auto& num : rix) {
			num = static_cast<T>(generate_random(low, high, false));
		}
	}

	void fill_random() {
		for (auto& num : rix) {
			num = static_cast<double>(generate_random(0, 0, true));
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

	bool dims_equal(const Matrix<T>& other) const {
		return this->rows == other.rows && this->cols == other.cols;
	}

	bool operator==(const Matrix<T>& other) const {
		if (dims_equal(other)) {
			for (int i = 0; i < this->rix.size(); i++) {
				if (this->rix[i] != other.rix[i]) return false;
			}
			return true;
		}
		return false;
	}

	Matrix<T> operator+(const Matrix<T>& other) const {
		if (dims_equal(other)) {
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
		if (dims_equal(other)) {
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
int trace(Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");
	int total{ 0 };
	for (int i = 0; i < mat.rows; i++) {
		total += mat(i, i);
	}
	return total;
}

template <typename T>
Matrix<T> submatrix(const Matrix<T>& mat, int exclude_row, int exclude_col) {
	Matrix<T> result(mat.rows - 1, mat.cols - 1);
	result.rix.clear();

	for (int i = 0; i < mat.rows; i++) {
		if (i == exclude_row) continue;

		for (int j = 0; j < mat.cols; j++) {
			if (j == exclude_col) continue;

			result.rix.push_back(mat(i, j));
		}
	}
	return result;
}

template <typename T>
Matrix<T> submatrix(const Matrix<T>& mat, std::set<int> exclude_rows, std::set<int> exclude_cols) {

	if (exclude_rows.size() > mat.rows || exclude_cols.size() > mat.cols)
		throw std::invalid_argument("Exclusion set size exceeded matrix dimensions.");

	Matrix<T> result(mat.rows - exclude_rows.size(), mat.cols - exclude_cols.size());
	result.rix.clear();

	for (int i = 0; i < mat.rows; i++) {
		if (exclude_rows.find(i) != exclude_rows.end()) continue;

		for (int j = 0; j < mat.cols; j++) {
			if (exclude_cols.find(j) != exclude_cols.end()) continue;

			result.rix.push_back(mat(i, j));
		}
	}
	return result;
}

template <typename T>
int det(Matrix<T>& mat) {
	if (mat.is_square() == 0) throw std::invalid_argument("Matrix must be a square matrix.");

	if (mat.rows == 2) return mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1);
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

template <typename T>
std::unordered_map<int, std::vector<int>> exclusions_for_square(Matrix<T>& mat) {
	if (mat.is_square())
		return NULL;
	else if (mat.rows > mat.cols)
		return std::pair(mat.rows - mat.cols, 0);
	else
		return std::pair(0, mat.cols - mat.rows);

	//int max_square_dim = mat.rows > mat.cols ? mat.cols : mat.rows;
}

template <typename T>
std::vector<Vector<T>> mat_to_vec(const Matrix<T>& mat) {
	std::vector<T> tmp;
	std::vector<Vector<T>> result;

	for (size_t col = 0; col < mat.cols; i++) 
	{
		for (size_t row = 0; row < mat.rows; j++) 
		{
			tmp.push_back(mat(row, col));
		}
		result.push_back(Vector<T>(tmp));
		tmp.clear();
	}
	return result;
}

template <typename T>
int gram_schmidt(Matrix<T>& mat) {

}

template <typename T>
bool is_lin_independent(const Matrix<T>& mat) {
	return det(mat);
}