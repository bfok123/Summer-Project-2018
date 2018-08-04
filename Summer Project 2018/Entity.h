#include "MathStuff.h"

struct Entity {
	float x, y, z;
	float width, height, length;

	Entity(float x, float y, float z, float width, float height, float length);
	bool collision(Entity other);
	Vector getPositionVector();
};
