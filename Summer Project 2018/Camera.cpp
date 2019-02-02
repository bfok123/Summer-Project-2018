#include "Camera.h"
#include <iostream>

Camera::Camera(Vector position) : position(position), yaw(YAW), pitch(PITCH), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
	worldUp = Vector(0.0f, 1.0f, 0.0f);
	updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;

	Vector scaleFront = front;
	Vector scaleRight = right;

	// to lock the player on xz axis
	scaleFront.elements[Coordinates::Y] = 0;
	scaleRight.elements[Coordinates::Y] = 0;

	scaleFront.scale(velocity);
	scaleRight.scale(velocity);

	if (direction == FORWARD) position += scaleFront;
	if (direction == BACKWARD) position -= scaleFront;
	if (direction == LEFT) position += scaleRight;
	if (direction == RIGHT) position -= scaleRight;
}

void Camera::processMouseMovement(double xOffset, double yOffset, bool constrainPitch) {
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch) {
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}
	
	updateCameraVectors();
}

Matrix Camera::getViewMatrix() {
	return lookAt(position, position + front, up);
}

void Camera::updateCameraVectors() {
	front.elements[Coordinates::X] = cos(degreesToRadians(yaw)) * cos(degreesToRadians(pitch));
	front.elements[Coordinates::Y] = sin(degreesToRadians(pitch));
	front.elements[Coordinates::Z] = sin(degreesToRadians(yaw)) * cos(degreesToRadians(pitch));
	front.normalize();

	right = worldUp * front;
	right.normalize();
	up = front * right;
	up.normalize();
}