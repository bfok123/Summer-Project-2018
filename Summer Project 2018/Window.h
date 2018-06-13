#ifndef WINDOW
#define WINDOW

#include "GLFW\glfw3.h"

struct Window {
	static GLFWwindow* window;
	static const char* title;
	static int width, height, widthRes, heightRes;
	static float xScale, yScale;
	//Function that OpenGL will use to keep the window state updated (screen resolution and ratio)
	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	

};


#endif