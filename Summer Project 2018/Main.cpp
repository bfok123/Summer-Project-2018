#include "Vector.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include "Window.h"

bool running;

void init(int windowWidth, int windowHeight, int windowScale, const char* title) {
	glfwInit();
	Window::window = glfwCreateWindow(Window::width = Window::widthRes = windowWidth,
									  Window::height = Window::heightRes = windowHeight, 
									  Window::title = title, NULL, NULL);


	//GLFW STUFF
	glfwMakeContextCurrent(Window::window);
	int width, height;
	glfwGetFramebufferSize(Window::window, &width, &height);
	glfwSwapInterval(0); //V-Sync stuff
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//Sets the center of the screen
	int xPos = (mode->width - (width * windowScale)) / 2;
	int yPos = (mode->height - (height * windowScale)) / 2;
	glfwSetWindowPos(Window::window, xPos, yPos);

	//Callbacks
	// TODO

	//GL (Honestly, just do this once, never understood
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Specific ID that the GPU program runs on
	glOrtho(0, width, 0, height, -10, 10); //This is camera stuff; -10 and 10 is for layering
	glDepthRange(-10, 10);
	glMatrixMode(GL_MODELVIEW);

	//^^^ IDK HOW MUCH OF THIS IS RELEVANT FOR 3D


	running = true;
}

void update() {
	running = !glfwWindowShouldClose(Window::window);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	
	glEnd();
}

void loop() {
	while (running) {
		update();
		render();
	}
}

int main() {
	int width = 1600;
	int height = 900;
	int scale = 1;
	const char* title = "boop"; //why tf isn't this working
	init(width, height, scale, title);
	loop();
}