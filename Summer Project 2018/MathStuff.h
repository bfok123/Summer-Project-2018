#ifndef MATH_STUFF
#define MATH_STUFF
#define _USE_MATH_DEFINES
#include <math.h>

enum Coords {
	X, Y, Z
};

inline static float degreesToRadians(float deg) { return deg * (M_PI / 180.0); }

struct Point {
	float x, y, z;

	Point(float x, float y, float z);
};

struct Vector {
	float *elements;

	Vector(float x, float y, float z);
	Vector* operator+(Vector& v);
	Vector* operator-(Vector& v);
	Vector* operator*(Vector& v); // cross product bc mark likes to do bad operator overloading
	Vector* operator*(float scalar);
	float dot(Vector& v);
	void rotate(float xy, float yz, float xz);
	Vector* translate(float x, float y, float z);
	Vector* translate(Vector& v);
	
	static float magnitude(Vector* v);
	static Vector* normalize(Vector* v);
};

struct Matrix {
	float *elements;
	int rows, cols;
	
	Matrix(int rows, int cols);

	Matrix* operator+(Matrix& m);
	Matrix* operator-(Matrix& m);
	Matrix* operator*(Matrix& m);
	Vector* operator*(Vector& m);
	Matrix* operator*(float scalar);

	static Matrix* rotate(Matrix* m, float angle, Vector* rotationAxis);
	static Matrix* translate(Matrix* m, float x, float y, float z);
	static Matrix* translate(Matrix* m, Vector& v);
	static Matrix* transpose(Matrix* m);
	static Matrix* identity(Matrix* m);
	static Matrix* zero(Matrix* m);
};

#endif