#include "MathStuff.h"
#include <iostream>

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

Vector::Vector() {
	Vector(0, 0, 0);
}

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
	this->elements[Coordinates::X] = this->elements[Coordinates::X] + right.elements[Coordinates::X];
	this->elements[Coordinates::Y] = this->elements[Coordinates::Y] + right.elements[Coordinates::Y];
	this->elements[Coordinates::Z] = this->elements[Coordinates::Z] + right.elements[Coordinates::Z];
	return *this;
}

Vector operator-(Vector left, const Vector& right) {
	left -= right;
	return left;
}

Vector& Vector::operator-=(const Vector& right) {
	this->elements[Coordinates::X] = this->elements[Coordinates::X] - right.elements[Coordinates::X];
	this->elements[Coordinates::Y] = this->elements[Coordinates::Y] - right.elements[Coordinates::Y];
	this->elements[Coordinates::Z] = this->elements[Coordinates::Z] - right.elements[Coordinates::Z];
	return *this;
}

Vector operator*(Vector left, const Vector& right) {
	left *= right;
	return left;
}

Vector& Vector::operator*=(const Vector& right) {
	Vector result(0, 0, 0);
	result.elements[Coordinates::X] = this->elements[Coordinates::Y] * right.elements[Coordinates::Z] - this->elements[Coordinates::Z] * right.elements[Coordinates::Y];
	result.elements[Coordinates::Y] = -(this->elements[Coordinates::X] * right.elements[Coordinates::Z] - this->elements[Coordinates::Z] * right.elements[Coordinates::X]);
	result.elements[Coordinates::Z] = this->elements[Coordinates::X] * right.elements[Coordinates::Y] - this->elements[Coordinates::Y] * right.elements[Coordinates::X];
	std::swap(result.elements, this->elements);
	return *this;
}

float Vector::dot(Vector right) {
	return (this->elements[Coordinates::X] * right.elements[Coordinates::X]) + (this->elements[Coordinates::Y] * right.elements[Coordinates::Y]) + (this->elements[Coordinates::Z] * right.elements[Coordinates::Z]);
}

float Vector::magnitude() {
	return sqrt(this->elements[Coordinates::X] * this->elements[Coordinates::X] + this->elements[Coordinates::Y] * this->elements[Coordinates::Y] + this->elements[Coordinates::Z] * this->elements[Coordinates::Z]);
}

void Vector::normalize() {
	this->scale(1.0f / this->magnitude());
}

void Vector::scale(float scalar) {
	elements[Coordinates::X] *= scalar;
	elements[Coordinates::Y] *= scalar;
	elements[Coordinates::Z] *= scalar;
}

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
	elements[rows * cols];
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			elements[j + i * rows] = 0;
		}
	}
	/*
	// make identity matrix if is square matrix CAUSED PROBLEMS WITH MATRIX MULTIPLICATION, NOT SURE IF KEEP
	if (rows == cols) {
		for (int i = 0; i < rows; i++) {
			elements[i + i * cols] = 1;
		}
	}*/
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
	if (this->cols == right.rows) {
		for (int i = 0; i < right.cols; i++) {
			for (int j = 0; j < this->rows; j++) {
				for (int k = 0; k < right.rows; k++) {
					result.elements[j + i * this->rows] += this->elements[j + k * this->rows] * right.elements[k + i * right.cols];
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
		for (int i = 0; i < 4; i++) {
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

void Matrix::rotate(float angle, Vector rotationAxis) {
	
	if (this->rows == 4 && this->cols == 4) {
		angle = degreesToRadians(angle);
		rotationAxis.normalize();
		float x = rotationAxis.elements[Coordinates::X];
		float y = rotationAxis.elements[Coordinates::Y];
		float z = rotationAxis.elements[Coordinates::Z];
		Matrix result(4, 4);
		result.elements[0] = cos(angle) + (x * x) * (1 - cos(angle));
		result.elements[1] = y * x * (1 - cos(angle)) + (z * sin(angle));
		result.elements[2] = z * x * (1 - cos(angle)) - (y * sin(angle));

		result.elements[4] = x * y * (1 - cos(angle)) - (z * sin(angle));
		result.elements[5] = cos(angle) + (y * y) * (1 - cos(angle));
		result.elements[6] = z * y * (1 - cos(angle)) + (x * sin(angle));

		result.elements[8] = x * z * (1 - cos(angle)) + (y * sin(angle));
		result.elements[9] = y * z * (1 - cos(angle)) - (x * sin(angle));
		result.elements[10] = cos(angle) + z * z * (1 - cos(angle));

		result.elements[15] = 1;
		/*
		Matrix result(4, 4);
		result.identity();
		rotationAxis.normalize();
		
		Matrix tensorProd(3, 3);
		tensorProd.elements[0] = rotationAxis.elements[Coordinates::X] * rotationAxis.elements[Coordinates::X];
		tensorProd.elements[1] = rotationAxis.elements[Coordinates::X] * rotationAxis.elements[Coordinates::Y];
		tensorProd.elements[2] = rotationAxis.elements[Coordinates::X] * rotationAxis.elements[Coordinates::Z];
		tensorProd.elements[3] = rotationAxis.elements[Coordinates::X] * rotationAxis.elements[Coordinates::Y];
		tensorProd.elements[4] = rotationAxis.elements[Coordinates::Y] * rotationAxis.elements[Coordinates::Y];
		tensorProd.elements[5] = rotationAxis.elements[Coordinates::Y] * rotationAxis.elements[Coordinates::Z];
		tensorProd.elements[6] = rotationAxis.elements[Coordinates::X] * rotationAxis.elements[Coordinates::Z];
		tensorProd.elements[7] = rotationAxis.elements[Coordinates::Y] * rotationAxis.elements[Coordinates::Z];
		tensorProd.elements[8] = rotationAxis.elements[Coordinates::Z] * rotationAxis.elements[Coordinates::Z];
		
		// Yeah, I hard coded this bc I don't know how it's derived
		Matrix crossProductMat(3, 3);
		crossProductMat.elements[1] = rotationAxis.elements[Coordinates::Z];
		crossProductMat.elements[2] = -rotationAxis.elements[Coordinates::Y];
		crossProductMat.elements[3] = -rotationAxis.elements[Coordinates::Z];
		crossProductMat.elements[5] = rotationAxis.elements[Coordinates::X];
		crossProductMat.elements[6] = rotationAxis.elements[Coordinates::Y];
		crossProductMat.elements[7] = -rotationAxis.elements[Coordinates::X];
		
		Matrix rotation(3, 3);
		rotation.identity();
		rotation.scale((float)cos(angle));
		crossProductMat.scale((float)sin(angle));
		tensorProd.scale(1.0f - (float)cos(angle));
		
		rotation = rotation + crossProductMat + tensorProd;
		
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.elements[j + i * 4] = rotation.elements[j + i * 3];
			}
		}*/
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
		result.identity();
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

void Matrix::translate(Vector v) {
	translate(v.elements[Coordinates::X], v.elements[Coordinates::Y], v.elements[Coordinates::Z]);
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

void Matrix::scale(float x, float y, float z) {
	Matrix scaleMat(4, 4);
	scaleMat.identity();
	scaleMat.elements[0] = x;
	scaleMat.elements[5] = y;
	scaleMat.elements[10] = z;
	Matrix result = *this;
	result *= scaleMat;
	std::swap(this->elements, result.elements);
}

// should i just manually input the elements to make this more efficient?????
void Matrix::ortho(float left, float right, float top, float bottom, float near, float far) {
	if (this->rows == 4 && this->cols == 4) {
		Matrix scale(4, 4);
		scale.identity();
		scale.scale(2.0f / (right - left), 2.0f / (top - bottom), 2.0f / (far - near));
		Matrix translate(4, 4);
		translate.identity();
		translate.translate(-((left + right) / 2.0f), -((top + bottom) / 2.0f), -((far + near) / 2.0f));
		translate.elements[10] = -1;
		Matrix result(4, 4);
		result = scale * translate;
		for (int i = 0; i < 16; i++) {
			std::cout << result.elements[i] << " ";
		}
		std::swap(this->elements, result.elements);
	} else {
		std::cout << "Given matrix must be 4x4 to turn into orthographic projection matrix.";
		std::cout << std::endl;
	}
}

void Matrix::perspective(float fieldOfView, float aspect, float near, float far) {
	if (this->rows == 4 && this->cols == 4) {
		// fieldOfView = degreesToRadians(fieldOfView);
		Matrix result(4, 4);
		// result.identity();
		// float f = 1.0f / tanf(fieldOfView * M_PI / 360.0f);
		float f = 1.0f / tanf(fieldOfView / 2.0f);

		result.elements[0] = f / aspect;
		result.elements[5] = f;
		result.elements[10] = (far + near) / (near - far);
		result.elements[11] = -1.0f;
		result.elements[14] = (2.0f * far * near) / (near - far);
		std::swap(this->elements, result.elements);
	} else {
		std::cout << "Given matrix must be 4x4 to turn into (symmetric) perspective projection matrix.";
		std::cout << std::endl;
	}
}

Matrix lookAt(Vector eye, Vector target, Vector up) {
	Vector fwd = target - eye; // 0 0 1
	fwd.normalize();
	Vector side = fwd * up; // 1 0 0
	side.normalize();
	Vector y = fwd * side;
	Matrix result(4, 4);
	result.elements[0] = side.elements[Coordinates::X];
	result.elements[1] = y.elements[Coordinates::X];
	result.elements[2] = -fwd.elements[Coordinates::X];
	result.elements[4] = side.elements[Coordinates::Y];
	result.elements[5] = y.elements[Coordinates::Y];
	result.elements[6] = -fwd.elements[Coordinates::Y];
	result.elements[8] = side.elements[Coordinates::Z];
	result.elements[9] = y.elements[Coordinates::Z];
	result.elements[10] = -fwd.elements[Coordinates::Z];
	result.elements[12] = -side.dot(eye);
	result.elements[13] = -y.dot(eye);
	result.elements[14] = -fwd.dot(eye);
	result.elements[15] = 1;
	return result;
}

//void 




