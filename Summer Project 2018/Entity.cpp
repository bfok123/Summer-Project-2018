#include "Entity.h"

Entity::Entity(float x, float y, float z, float height, float width, float length) : x(x), y(y), z(z), height(height), width(width), length(length) {}

bool Entity::collision(Entity other) {
	float thisMinX = x - (width / 2.0f);
	float thisMaxX = x + (width / 2.0f);
	float thisMinY = y - (height / 2.0f);
	float thisMaxY = y + (height / 2.0f);
	float thisMinZ = z - (length / 2.0f);
	float thisMaxZ = z + (length / 2.0f);
	
	float otherMinX = other.x - (other.width / 2.0f);
	float otherMaxX = other.x + (other.width / 2.0f);
	float otherMinY = other.y - (other.height / 2.0f);
	float otherMaxY = other.y + (other.height / 2.0f);
	float otherMinZ = other.z - (other.length / 2.0f);
	float otherMaxZ = other.z + (other.length / 2.0f);

	return (thisMinX <= otherMaxX && thisMaxX >= otherMinX) &&
		   (thisMinY <= otherMaxY && thisMaxY >= otherMinY) &&
		   (thisMinZ <= otherMaxZ && thisMaxZ >= otherMinZ);
}

Vector Entity::getPositionVector() {
	return Vector(x, y, z);
}