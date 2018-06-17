#include "MathStuff.h"
#include <iostream>

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

Vector::Vector(float x, float y, float z) {
	elements[4];
	elements[0] = x;
	elements[1] = y;
	elements[2] = z;
	elements[3] = 1;
} 

Vector operator+(Vector left, const Vector& right) {
	left += right;
	return left;
}

Vector& Vector::operator+=(const Vector& right) {
	this->elements[Coords::X] = this->elements[Coords::X] + right.elements[Coords::X];
	this->elements[Coords::Y] = this->elements[Coords::Y] + right.elements[Coords::Y];
	this->elements[Coords::Z] = this->elements[Coords::Z] + right.elements[Coords::Z];
	return *this;
}

Vector operator-(Vector left, const Vector& right) {
	left -= right;
	return left;
}

Vector& Vector::operator-=(const Vector& right) {
	this->elements[Coords::X] = this->elements[Coords::X] - right.elements[Coords::X];
	this->elements[Coords::Y] = this->elements[Coords::Y] - right.elements[Coords::Y];
	this->elements[Coords::Z] = this->elements[Coords::Z] - right.elements[Coords::Z];
	return *this;
}

Vector& Vector::operator*=(const Vector& right) {
	this->elements[Coords::X] = this->elements[Coords::Y] * right.elements[Coords::Z] - this->elements[Coords::Z] * right.elements[Coords::Y];
	this->elements[Coords::Y] = -(this->elements[Coords::X] * right.elements[Coords::Z] - this->elements[Coords::Z] * right.elements[Coords::X]);
	this->elements[Coords::Z] = this->elements[Coords::X] * right.elements[Coords::Y] - this->elements[Coords::Y] * right.elements[Coords::X];
	return *this;
}

float Vector::dot(Vector& right) {
	return this->elements[Coords::X] * right.elements[Coords::X] + this->elements[Coords::Y] * right.elements[Coords::Y] + this->elements[Coords::Z] * right.elements[Coords::Z];
}

float Vector::magnitude() {
	return sqrt(this->elements[Coords::X] * this->elements[Coords::X] + this->elements[Coords::Y] * this->elements[Coords::Y] + this->elements[Coords::Z] * this->elements[Coords::Z]);
}

void Vector::normalize() {
	this->scale(1.0f / this->magnitude());
}

void Vector::scale(float scalar) {
	elements[Coords::X] *= scalar;
	elements[Coords::Y] *= scalar;
	elements[Coords::Z] *= scalar;
}

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
	elements[rows * cols];
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			elements[j + i * rows] = 0;
		}
	}
	
	// make identity matrix if is square matrix CAUSED PROBLEMS WITH MATRIX MULTIPLICATION, NOT SURE IF KEEP
	if (rows == cols) {
		for (int i = 0; i < rows; i++) {
			elements[i + i * cols] = 1;
		}
	}
}

Matrix& Matrix::operator=(Matrix right) {
	std::swap(this->elements, right.elements);
	std::swap(this->rows, right.rows);
	std::swap(this->cols, right.cols);
	return *this;
}

Matrix operator+(Matrix left, const Matrix& right) {
	left += right;
	return left;
}

Matrix& Matrix::operator+=(const Matrix& right) {
	if (this->rows == right.rows && this->cols == right.cols) {
		for (int i = 0; i < this->cols; i++) {
			for (int j = 0; j < this->rows; j++) {
				this->elements[j + i * this->rows] = this->elements[j + i * this->rows] + right.elements[j + i * right.rows];
			}
		}
	} else {
		std::cout << "Rows and columns must be equal to add matrices.";
		std::cout << std::endl;
	}
	return *this;
}

Matrix operator-(Matrix left, const Matrix& right) {
	left -= right;
	return left;
}

Matrix& Matrix::operator-=(const Matrix& right) {
	if (this->rows == right.rows && this->cols == right.cols) {
		for (int i = 0; i < this->cols; i++) {
			for (int j = 0; j < this->rows; j++) {
				this->elements[j + i * this->rows] = this->elements[j + i * this->rows] - right.elements[j + i * right.rows];
			}
		}
	} else {
		std::cout << "Rows and columns must be equal to subtract matrices.";
		std::cout << std::endl;
	}
	return *this;
}

Matrix operator*(Matrix left, const Matrix& right) {
	left *= right;
	return left;
}

Matrix& Matrix::operator*=(const Matrix& right) {
	Matrix result(this->rows, right.cols);
	result.zero();
	if (this->cols == right.rows) {
		for (int i = 0; i < right.cols; i++) {
			for (int j = 0; j < this->rows; j++) {
				for (int k = 0; k < right.rows; k++) {
					result.elements[j + i * this->rows] += this->elements[j + k * this->rows] * right.elements[i + k * right.cols];
				}
			}
		}

		std::swap(this->elements, result.elements);
		this->cols = right.cols;
	} else {
		std::cout << "Columns must be equal to rows of other matrix to multiply matrices.";
		std::cout << std::endl;
	}
	return *this;
}

Vector operator*(Matrix left, const Vector& right) {
	Vector result(0, 0, 0);
	if (left.rows == 4 && left.cols == 4) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				result.elements[i] += right.elements[j] * left.elements[i + j * left.cols];
			}
		}
	} else {
		std::cout << "Must multiply vector by a 4x4 matrix.";
		std::cout << std::endl;
	}
	return result;
}

void Matrix::rotate(float angle, float x, float y, float z) {
	Vector rotationAxis(x, y, z);
	Matrix::rotate(angle, rotationAxis);
}

void Matrix::rotate(float angle, Vector& rotationAxis) {
	if (this->rows == 4 && this->cols == 4) {
		angle = degreesToRadians(angle);
		Matrix result(4, 4);
		// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		rotationAxis.normalize();

		// Rotation matrix
		Matrix u(3, 1);
		for (int i = 0; i < 3; i++) {
			u.elements[i] = rotationAxis.elements[i];
		}

		Matrix tensorProd(3, 3);
		Matrix uOrig(3, 1);
		uOrig = u;
		u.transpose();
		for (int i = 0; i < 3; i++) {
			std::cout << u.elements[i] << " ";
		}
		tensorProd = uOrig * u;
		
		// Yeah, I hard coded this bc I don't know how it's derived
		Matrix crossProductMat(3, 3);
		crossProductMat.zero();
		crossProductMat.elements[1] = rotationAxis.elements[Coords::Z];
		crossProductMat.elements[2] = -rotationAxis.elements[Coords::Y];
		crossProductMat.elements[3] = -rotationAxis.elements[Coords::Z];
		crossProductMat.elements[5] = rotationAxis.elements[Coords::X];
		crossProductMat.elements[6] = rotationAxis.elements[Coords::Y];
		crossProductMat.elements[7] = -rotationAxis.elements[Coords::X];
		
		Matrix rotation(3, 3);
		rotation.scale((float)cos(angle));
		crossProductMat.scale((float)sin(angle));
		for (int i = 0; i < 9; i++) {
			std::cout << tensorProd.elements[i] << " ";
		}
		tensorProd.scale(1.0f - (float)cos(angle));
		
		rotation = rotation + crossProductMat + tensorProd;
		
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.elements[j + i * 4] = rotation.elements[j + i * 3];
			}
		}
		std::cout << std::endl;
		result *= *this;
		std::swap(this->elements, result.elements);
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into rotation matrix.";
		std::cout << std::endl;
	}
}

void Matrix::translate(float x, float y, float z) {
	if (this->rows == 4 && this->cols == 4) {
		Matrix result(4, 4);
		result.elements[12] = x;
		result.elements[13] = y;
		result.elements[14] = z;
		result *= *this;
		std::swap(this->elements, result.elements);
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into translation matrix.";
		std::cout << std::endl;
	}
}

void Matrix::translate(Vector& v) {
	if (this->rows == 4 && this->cols == 4) {
		Matrix result(4, 4);
		for (int i = 0; i < 3; i++) {
			result.elements[i + 3 * 4] = v.elements[i];
		}
		result *= *this;
		std::swap(this->elements, result.elements);
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into translation matrix.";
		std::cout << std::endl;
	}
}

/*
	IF THERE IS EVER A PROBLEM IT COULD BE THIS FUNCTION
*/
void Matrix::transpose() {
	Matrix result(this->cols, this->rows);
	for (int i = 0; i < result.cols; i++) {
		for (int j = 0; j < result.rows; j++) {
			result.elements[i + j * result.rows] = this->elements[j + i * this->cols];
		}
	}
	std::swap(this->elements, result.elements);
	this->rows = result.rows;
	this->cols = result.cols;
}

void Matrix::identity() {
	if (this->rows == this->cols) {
		for (int i = 0; i < this->cols; i++) {
			this->elements[i + i * this->cols] = 1;
		}
	}
	else {
		std::cout << "Given matrix must be a square matrix in order to create an identity matrix.";
		std::cout << std::endl;
	}
}

void Matrix::zero() {
	for (int i = 0; i < this->cols; i++) {
		for (int j = 0; j < this->rows; j++) {
			this->elements[i + j * this->rows] = 0;
		}
	}
}

void Matrix::scale(float scalar) {
	for (int i = 0; i < rows * cols; i++) {
		elements[i] *= scalar;
	}
}




