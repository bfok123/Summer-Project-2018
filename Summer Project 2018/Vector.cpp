#include "Vector.h"

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}
Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

Vector* Vector::operator+(Vector& v) {
	Vector* result = new Vector(0, 0, 0);
	result->x = this->x + v.x;
	result->y = this->y + v.y;
	result->z = this->z + v.z;
	return result;
}

Vector* Vector::operator-(Vector& v) {
	Vector* result = new Vector(0, 0, 0);
	result->x = this->x - v.x;
	result->y = this->y - v.y;
	result->z = this->z - v.z;
	return result;
}

void Vector::scale(float scaleX, float scaleY, float scaleZ) {
	x *= scaleX;
	y *= scaleY;
	z *= scaleZ;
}

void Vector::rotate(float xy, float yz, float xz) {
	xy = degreesToRadians(xy);
	yz = degreesToRadians(yz);
	xz = degreesToRadians(xz);

	// XY rotation
	x = x * cos(xy) - y * sin(xy);
	y = x * sin(xy) + y * cos(xy);

	// YZ rotation
	y = y * cos(yz) - z * sin(yz);
	z = y * sin(yz) + z * cos(yz);

	// XZ rotation
	x = x * cos(xz) + z * sin(xz);
	z = x * -sin(xz) + z * cos(xz);
}

