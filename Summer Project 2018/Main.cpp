#include "Vector.h"
#include <iostream>
#include "GLFW\glfw3.h"

bool running;

void init() {
	running = true;
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(50, 50, "boop", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, 50, 50);
}

void update() {

}

void render() {

}

void loop() {
	while (running) {
		update();
		render();
	}
}

int main() {
	
	
}