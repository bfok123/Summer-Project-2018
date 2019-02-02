#ifndef CAMERA
#define CAMERA
#include "MathStuff.h"

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

struct Camera {
	Vector position, front, up, right, worldUp;
	float yaw, pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(Vector position);
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(double xOffset, double yOffset, bool constrainPitch = true);
	Matrix getViewMatrix();
	void updateCameraVectors();
};

#endif