#include "MathStuff.h"
#include <iostream>

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

Vector::Vector(float x, float y, float z) {
	elements = new float[4];
	elements[0] = x;
	elements[1] = y;
	elements[2] = z;
	elements[3] = 1;
} 

Vector* Vector::operator+(Vector& v) {
	Vector* result = new Vector(0, 0, 0);
	result->elements[Coords::X] = this->elements[Coords::X] + v.elements[Coords::X];
	result->elements[Coords::Y] = this->elements[Coords::Y] + v.elements[Coords::Y];
	result->elements[Coords::Z] = this->elements[Coords::Z] + v.elements[Coords::Z];
	return result;
}

Vector* Vector::operator-(Vector& v) {
	Vector* result = new Vector(0, 0, 0);
	result->elements[Coords::X] = this->elements[Coords::X] - v.elements[Coords::X];
	result->elements[Coords::Y] = this->elements[Coords::Y] - v.elements[Coords::Y];
	result->elements[Coords::Z] = this->elements[Coords::Z] - v.elements[Coords::Z];
	return result;
}

Vector* Vector::operator*(Vector& v) {
	Vector* result = new Vector(0, 0, 0);
	result->elements[Coords::X] = this->elements[Coords::Y] * v.elements[Coords::Z] - this->elements[Coords::Z] * v.elements[Coords::Y];
	result->elements[Coords::Y] = -(this->elements[Coords::X] * v.elements[Coords::Z] - this->elements[Coords::Z] * v.elements[Coords::X]);
	result->elements[Coords::Z] = this->elements[Coords::X] * v.elements[Coords::Y] - this->elements[Coords::Y] * v.elements[Coords::X];
	return result;
}

Vector* Vector::operator*(float scalar) {
	Vector* result = new Vector(0, 0, 0);
	result->elements[Coords::X] = scalar * this->elements[Coords::X];
	result->elements[Coords::Y] = scalar * this->elements[Coords::Y];
	result->elements[Coords::Z] = scalar * this->elements[Coords::Z];
	return result;
}

float Vector::dot(Vector& v) {
	return this->elements[Coords::X] * v.elements[Coords::X] + this->elements[Coords::Y] * v.elements[Coords::Y] + this->elements[Coords::Z] * v.elements[Coords::Z];
}

void Vector::rotate(float xy, float yz, float xz) {
	xy = degreesToRadians(xy);
	yz = degreesToRadians(yz);
	xz = degreesToRadians(xz);

	// XY rotation
	elements[Coords::X] = elements[Coords::X] * cos(xy) - elements[Coords::Y] * sin(xy);
	elements[Coords::Y] = elements[Coords::X] * sin(xy) + elements[Coords::Y] * cos(xy);

	// YZ rotation
	elements[Coords::Y] = elements[Coords::Y] * cos(yz) - elements[Coords::Z] * sin(yz);
	elements[Coords::Z] = elements[Coords::Y] * sin(yz) + elements[Coords::Z] * cos(yz);

	// XZ rotation
	elements[Coords::X] = elements[Coords::X] * cos(xz) + elements[Coords::Z] * sin(xz);
	elements[Coords::Z] = elements[Coords::X] * -sin(xz) + elements[Coords::Z] * cos(xz);
}

float Vector::magnitude(Vector* v) {
	return sqrt(v->elements[Coords::X] * v->elements[Coords::X] + v->elements[Coords::Y] * v->elements[Coords::Y] + v->elements[Coords::Z] * v->elements[Coords::Z]);
}

Vector* Vector::normalize(Vector* v) {
	return *v * (1.0f / Vector::magnitude(v));
}

// should I keep this here?? already can do this with matrices, them multiply vector by transformation matrices (advantage is that you can do multiple transformations at once)
Vector* Vector::translate(float x, float y, float z) {
	return nullptr;
}

Vector* Vector::translate(Vector& v) {
	return nullptr;
}

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
	elements = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			elements[i + j * cols] = 0;
		}
	}
	
	// make identity matrix if is square matrix CAUSED PROBLEMS WITH MATRIX MULTIPLICATION, NOT SURE IF KEEP
	if (rows == cols) {
		for (int i = 0; i < rows; i++) {
			elements[i + i * cols] = 1;
		}
	}
}

Matrix& Matrix::add(const Matrix& m) {
	for (int i = 0; i < this->rows * this->cols; i++) {
		this->elements[i] += m.elements[i];
	}
	return *this;
}

Matrix& Matrix::operator=(Matrix& m) {
	Matrix* result = new Matrix(m.rows, m.cols);
	for (int i = 0; i < m.rows * m.cols; i++) {
		result->elements[i] = m.elements[i];
	}
	std::cout << "oh hi mark";
	return *result;
}

Matrix& operator+(Matrix& m1, const Matrix& m2) {
	/*
	if (m1.rows == m2.rows && m1.cols == m2.cols) {
		for (int i = 0; i < m1.rows; i++) {
			for (int j = 0; j < m1.cols; j++) {
				m1.elements[i + j * m1.cols] = m1.elements[i + j * m1.cols] + m2.elements[i + j * m2.cols];
			}
		}
		std::cout << "oh hi mark 2";
	}
	else {
		std::cout << "Rows and columns must be equal to add matrices.";
		std::cout << std::endl;
	}
	return *this; */
	return m1.add(m2);
}


Matrix* Matrix::operator-(Matrix& m) {
	if (this->rows == m.rows && this->cols == m.cols) {
		Matrix* result = new Matrix(this->rows, this->cols);
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < this->cols; j++) {
				result->elements[i + j * result->cols] = this->elements[i + j * this->cols] - m.elements[i + j * m.cols];
			}
		}
		return result;
	}
	else {
		std::cout << "Rows and columns must be equal to subtract matrices.";
		std::cout << std::endl;
		return nullptr;
	}
}

Matrix* Matrix::operator*(Matrix& m) {
	if (this->cols == m.rows) {
		Matrix* result = new Matrix(this->rows, m.cols);
		result->zero();
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < m.cols; j++) {
				for (int k = 0; k < m.rows; k++) {
					result->elements[i + j * result->cols] += m.elements[k + j * m.cols] * this->elements[i + k * this->cols];
				}
			}
		}
		return result;
	}
	else {
		std::cout << "Columns must be equal to rows of other matrix to multiply matrices.";
		std::cout << std::endl;
		return nullptr;
	}
}

Vector* Matrix::operator*(Vector& v) {
	if (rows == 4 && cols == 4) {
		Vector* result = new Vector(0, 0, 0);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				result->elements[i] += v.elements[j] * this->elements[i + j * cols];
			}
		}
		return result;
	}
	else {
		std::cout << "Must multiply vector by a 4x4 matrix.";
		std::cout << std::endl;
		return nullptr;
	}
}

Matrix* Matrix::operator*(float scalar) {
	Matrix* result = new Matrix(this->rows, this->cols);
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			result->elements[i + j * cols] = scalar * this->elements[i + j * cols];
		}
	}
	return result;
}

void Matrix::rotate(float angle, Vector* rotationAxis) {
	if (this->rows == 4 && this->cols == 4) {
		angle = degreesToRadians(angle);
		Matrix* result = new Matrix(4, 4);
		// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		rotationAxis = Vector::normalize(rotationAxis);

		// Rotation matrix
		Matrix* u = new Matrix(3, 1);
		for (int i = 0; i < 3; i++) {
			u->elements[i] = rotationAxis->elements[i];
		}
		
		Matrix* tensorProd = u;
		std::cout << tensorProd->rows << " " << tensorProd->cols;
		std::cout << std::endl;
		u->transpose();
		std::cout << tensorProd->rows << " " << tensorProd->cols;
		tensorProd = *tensorProd * *u;
		
		// Yeah, I hard coded this bc I don't know how it's derived
		Matrix* crossProductMat = new Matrix(3, 3);
		crossProductMat->elements[1] = rotationAxis->elements[Coords::Z];
		crossProductMat->elements[2] = -rotationAxis->elements[Coords::Y];
		crossProductMat->elements[3] = -rotationAxis->elements[Coords::Z];
		crossProductMat->elements[5] = rotationAxis->elements[Coords::X];
		crossProductMat->elements[6] = rotationAxis->elements[Coords::Y];
		crossProductMat->elements[7] = -rotationAxis->elements[Coords::X];
		
		Matrix* rotation = new Matrix(3, 3);
		//rotation = *(*rotation * (float) cos(angle)) + *(*crossProductMat * (float) sin(angle));
		//rotation = *rotation + *(*tensorProd * (1.0f - (float) cos(angle)));

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result->elements[i + j * 4] = rotation->elements[i + j * 3];
			}
		}
		this->elements = (*result * *this)->elements;
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into rotation matrix.";
		std::cout << std::endl;
	}
}

void Matrix::translate(float x, float y, float z) {
	if (this->rows == 4 && this->cols == 4) {
		Matrix* result = new Matrix(4, 4);
		result->elements[12] = x;
		result->elements[13] = y;
		result->elements[14] = z;
		this->elements = (*result * *this)->elements;
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into translation matrix.";
		std::cout << std::endl;
	}
}

void Matrix::translate(Vector& v) {
	if (this->rows == 4 && this->cols == 4) {
		Matrix* result = new Matrix(4, 4);
		for (int i = 0; i < 3; i++) {
			result->elements[i + 3 * 4] = v.elements[i];
		}
		this->elements = (*result * *this)->elements;
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into translation matrix.";
		std::cout << std::endl;
	}
}

void Matrix::transpose() {
	Matrix* result = new Matrix(this->cols, this->rows);
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			result->elements[j + i * this->rows] = this->elements[i + j * this->cols];
		}
	}
	this->elements = result->elements;
	this->rows = result->rows;
	this->cols = result->cols;
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
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			this->elements[i + j * this->cols] = 0;
		}
	}
}




