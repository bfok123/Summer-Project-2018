#include "Camera.h"
#include <iostream>

Camera::Camera(Vector position = Vector()) : position(position), yaw(YAW), pitch(PITCH), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
	worldUp = Vector(0.0f, 1.0f, 0.0f);
	front = Vector(0.0f, 0.0f, -1.0f);
	up = Vector(0.0f, 1.0f, 0.0f);
	updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;

	Vector scaleFront = front;
	Vector scaleRight = right;
	scaleFront.scale(velocity);
	scaleRight.scale(velocity);

	if (direction == FORWARD) position -= scaleFront;
	if (direction == BACKWARD) position += scaleFront;
	if (direction == LEFT) position -= scaleRight;
	if (direction == RIGHT) position += scaleRight;
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
/*
void Camera::mouseUpdate(int xOffset, int yOffset) {
	Matrix rot(4, 4);
	rot.identity();
	rot.rotate(-(float)xOffset * mouseSensitivity, up);
	rot.rotate((float)yOffset * mouseSensitivity, right);
	front = rot * front;
	front.normalize();
}*/