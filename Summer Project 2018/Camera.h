#include "MathStuff.h"

struct Camera {
	Matrix* viewMatrix;
	float yaw, pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(float x, float y, float z);
};