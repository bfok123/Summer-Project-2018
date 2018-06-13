#include "Window.h"

GLFWwindow* Window::window = nullptr;
const char* Window::title = nullptr;
int Window::width = 0;
int Window::height = 0;
int Window::widthRes = 0;
int Window::heightRes = 0;
float Window::xScale = 0;
float Window::yScale = 0;

void Window::windowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, Window::width = width, Window::height = height);
	xScale = (float) width / widthRes;
	yScale = (float) height / heightRes;
}