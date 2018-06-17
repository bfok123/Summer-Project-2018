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
	//float *elements;
	float elements[16];
	int rows, cols;
	
	Matrix(int rows, int cols);

	Matrix& add(const Matrix& m);
	
	Matrix& operator=(Matrix& m);
	friend Matrix& operator+(Matrix& m1, const Matrix& m2); 
	Matrix* operator-(Matrix& m);
	Matrix* operator*(Matrix& m);
	Vector* operator*(Vector& m);
	Matrix* operator*(float scalar);

	void rotate(float angle, float x, float y, float z);
	void rotate(float angle, Vector* rotationAxis);
	void translate(float x, float y, float z);
	void translate(Vector& v);
	void transpose();
	void identity();
	void zero();
	void scale(float scalar); // uniform scale
	void scale(float x, float y, float z); // scale on each axis
};

#endif