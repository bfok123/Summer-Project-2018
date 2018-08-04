#include "Mouse.h"

double Mouse::x = 0;
double Mouse::y = 0;
double Mouse::xOffset = 0;
double Mouse::yOffset = 0;
bool Mouse::firstMouseMovement = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::mousePosCallback(GLFWwindow* window, double x, double y) {
	if (firstMouseMovement) {
		Mouse::x = x;
		Mouse::y = y;
		firstMouseMovement = false;
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	xOffset = x - Mouse::x;
	yOffset = y - Mouse::y;
	Mouse::x = x;
	Mouse::y = y;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int index, int action, int mods) {
	if (index < 0)
		return;
	buttons[index] = action == GLFW_PRESS;
}

void Mouse::zeroOffsets() {
	xOffset = 0;
	yOffset = 0;
}