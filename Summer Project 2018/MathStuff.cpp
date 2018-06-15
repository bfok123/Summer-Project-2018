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
			elements[j + i * cols] = 0;
		}
	}
	
	// make identity matrix if is square matrix CAUSED PROBLEMS WITH MATRIX MULTIPLICATION, NOT SURE IF KEEP
	if (rows == cols) {
		for (int i = 0; i < rows; i++) {
			elements[i + i * cols] = 1;
		}
	}
}

Matrix* Matrix::operator+(Matrix& m) {
	if (this->rows == m.rows && this->cols == m.cols) {
		Matrix* result = new Matrix(this->rows, this->cols);
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < this->cols; j++) {
				result->elements[j + i * result->cols] = this->elements[j + i * this->cols] + m.elements[j + i * m.cols];
			}
		}
		return result;
	}
	else {
		std::cout << "Rows and columns must be equal to add matrices.";
		return nullptr;
	}
	
}

Matrix* Matrix::operator-(Matrix& m) {
	if (this->rows == m.rows && this->cols == m.cols) {
		Matrix* result = new Matrix(this->rows, this->cols);
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < this->cols; j++) {
				result->elements[j + i * result->cols] = this->elements[j + i * this->cols] - m.elements[j + i * m.cols];
			}
		}
		return result;
	}
	else {
		std::cout << "Rows and columns must be equal to subtract matrices.";
		return nullptr;
	}
}

Matrix* Matrix::operator*(Matrix& m) {
	if (this->cols == m.rows) {
		Matrix* result = Matrix::zero(new Matrix(this->rows, m.cols));
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < m.cols; j++) {
				for (int k = 0; k < m.rows; k++) {
					result->elements[j + i * result->cols] += m.elements[j + k * m.cols] * this->elements[k + i * this->cols];
				}
			}
		}
		return result;
	}
	else {
		std::cout << "Columns must be equal to rows of other matrix to multiply matrices.";
		return nullptr;
	}
}

Vector* Matrix::operator*(Vector& v) {
	if (rows == 4 && cols == 4) {
		Vector* result = new Vector(0, 0, 0);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				result->elements[i] += v.elements[j] * this->elements[j + i * cols];
			}
		}
		return result;
	}
	else {
		std::cout << "Must multiply vector by a 4x4 matrix.";
		return nullptr;
	}
}

Matrix* Matrix::operator*(float scalar) {
	Matrix* result = new Matrix(this->rows, this->cols);
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			result->elements[j + i * cols] = scalar * this->elements[j + i * cols];
		}
	}
	return result;
}

Matrix* Matrix::rotate(Matrix* m, float angle, Vector* rotationAxis) {
	if (m->rows == 4 && m->cols == 4) {
		angle = degreesToRadians(angle);
		Matrix* result = new Matrix(4, 4);
		// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		rotationAxis = Vector::normalize(rotationAxis);

		// Rotation matrix
		Matrix* u = new Matrix(3, 1);
		for (int i = 0; i < 3; i++) {
			u->elements[i * u->cols] = rotationAxis->elements[i];
		}
		Matrix* tensorProd = *u * *Matrix::transpose(u);
		
		// Yeah, I hard coded this bc I don't know how it's derived
		Matrix* crossProductMat = new Matrix(3, 3);
		crossProductMat = Matrix::zero(crossProductMat);
		crossProductMat->elements[1] = -rotationAxis->elements[Coords::Z];
		crossProductMat->elements[2] = rotationAxis->elements[Coords::Y];
		crossProductMat->elements[3] = rotationAxis->elements[Coords::Z];
		crossProductMat->elements[5] = -rotationAxis->elements[Coords::X];
		crossProductMat->elements[6] = -rotationAxis->elements[Coords::Y];
		crossProductMat->elements[7] = rotationAxis->elements[Coords::X];
		
		Matrix* rotation = new Matrix(3, 3);
		rotation = *(*rotation * (float) cos(angle)) + *(*crossProductMat * (float) sin(angle));
		rotation = *rotation + *(*tensorProd * (1.0f - (float) cos(angle)));

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result->elements[j + i * 4] = rotation->elements[j + i * 3];
			}
		}
		return *result * *m;
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into rotation matrix.";
	}
}

Matrix* Matrix::translate(Matrix* m, float x, float y, float z) {
	if (m->rows == 4 && m->cols == 4) {
		Matrix* result = new Matrix(4, 4);
		result->elements[3] = x;
		result->elements[7] = y;
		result->elements[11] = z;
		return *result * *m;
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into translation matrix.";
		return nullptr;
	}
}

Matrix* Matrix::translate(Matrix* m, Vector& v) {
	if (m->rows == 4 && m->cols == 4) {
		Matrix* result = new Matrix(4, 4);
		for (int i = 0; i < 3; i++) {
			result->elements[3 + i * 4] = v.elements[i];
		}
		return *result * *m;
	}
	else {
		std::cout << "Given matrix must be 4x4 to turn into translation matrix.";
		return nullptr;
	}
}

Matrix* Matrix::transpose(Matrix* m) {
	Matrix* result = new Matrix(m->cols, m->rows);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			result->elements[i + j * m->rows] = m->elements[j + i * m->cols];
		}
	}
	return result;
}

// possibly take in rows and cols instead of a matrix?
Matrix* Matrix::identity(Matrix* m) {
	if (m->rows == m->cols) {
		Matrix* result = new Matrix(m->rows, m->cols);
		return result;
	}
	else {
		std::cout << "Given matrix must be a square matrix in order to create an identity matrix.";
		return nullptr;
	}
}

Matrix* Matrix::zero(Matrix* m) {
	Matrix* result = new Matrix(m->rows, m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			result->elements[j + i * m->cols] = 0;
		}
	}
	return result;
}


