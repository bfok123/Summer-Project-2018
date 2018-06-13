#ifndef VECTOR
#define VECTOR
#define _USE_MATH_DEFINES
#include <math.h>

inline static float degreesToRadians(float deg) { return deg * (M_PI / 180.0); }

struct Point {
	float x, y, z;

	Point(float x, float y, float z);
};

struct Vector {
	float x, y, z;

	Vector(float x, float y, float z);
	Vector* operator+(Vector& v);
	Vector* operator-(Vector& v);
	void scale(float scaleX, float scaleY, float scaleZ);
	void rotate(float xy, float yz, float xz);
};

#endif
