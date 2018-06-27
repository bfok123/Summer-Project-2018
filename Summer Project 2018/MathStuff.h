#ifndef MATH_STUFF
#define MATH_STUFF
#define _USE_MATH_DEFINES
#include <math.h>

enum Coordinates {
	X, Y, Z, W
};

inline static float degreesToRadians(float deg) { return deg * (M_PI / 180.0); }

struct Point {
	float x, y, z;

	Point(float x, float y, float z);
};

struct Vector {
	float elements[4];

	Vector(float x, float y, float z);

	//Vector& operator=(Vector right);
	Vector& operator+=(const Vector& right);
	Vector& operator-=(const Vector& right);
	Vector& operator*=(const Vector& right);

	friend Vector operator+(Vector left, const Vector& right);
	friend Vector operator-(Vector left, const Vector& right);
	friend Vector operator*(Vector left, const Vector& right); // cross product bc mark likes to do bad operator overloading

	float dot(Vector& right);
	float magnitude();
	void normalize();
	void scale(float scalar); // uniform scale
	void scale(float x, float y, float z); // scale on each axis
};

struct Matrix {
	float elements[16];
	int rows, cols;
	
	Matrix(int rows, int cols);

	Matrix& operator=(Matrix right);
	Matrix& operator+=(const Matrix& right);
	Matrix& operator-=(const Matrix& right);
	Matrix& operator*=(const Matrix& right);
	// operator==
	
	friend Matrix operator+(Matrix left, const Matrix& right);
	friend Matrix operator-(Matrix left, const Matrix& right);
	friend Matrix operator*(Matrix left, const Matrix& right);
	friend Vector operator*(Matrix left, const Vector& right);

	void rotate(float angle, float x, float y, float z);
	void rotate(float angle, Vector& rotationAxis);
	void translate(float x, float y, float z);
	void translate(Vector& v);
	void transpose();
	void identity();
	void zero();
	void scale(float scalar); // uniform scale
	void scale(float x, float y, float z); // scale on each axis
	void orthodontist(float left, float right, float top, float bottom, float near, float far);
};



#endif