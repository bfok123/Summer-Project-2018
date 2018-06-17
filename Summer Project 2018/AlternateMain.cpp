#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include "Vector.h"
#include "Window.h"
#include "Shader.h"

bool running;

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 450;
const unsigned int SCALE = 3;
const char* TITLE = "boop";

bool init() {

	//GLFW
	if (!glfwInit()) {
		std::cout << "GLFW failed to init! :(" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Configuring some stuff
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window::window = glfwCreateWindow(Window::width = Window::widthRes = WIDTH,
									  Window::height = Window::heightRes = HEIGHT, 
									  Window::title = TITLE, NULL, NULL);
	if (!Window::window) {
		std::cout << "Failed GLFW window creation :(" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(Window::window);

	//GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "GLEW failed to init! :(" << std::endl;
		std::cout << glewGetErrorString(error) << std::endl;
		return false;
	}

	//GL
	glViewport(0, 0, WIDTH, HEIGHT);

	//Callbacks
	glfwSetFramebufferSizeCallback(Window::window, Window::windowSizeCallback);
	
	//Created a Shader obect
	Shader shader("Shader.vertex", "Shader.fragment");

	//Vertex Buffer Object and Element Object Buffer Setup
	//TEST VERTICES FOR TRIANGLES
	//Triangle
	float vertices[] = { //(x, y, z, r, g, b)
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //Top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //Bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //Bottom left
	   -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f  //Top left
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //0, because layout location for position is 0
					  //ID  elements ... ... Stride of 6 * sizeof(float)     offset
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); //1, because layout location for color is 1

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	running = true;
	double timePassed = 0;
	while (!glfwWindowShouldClose(Window::window)) {
		glfwPollEvents();

		glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);




		shader.use();




		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(Window::window);
		
	}

	return running = true;
}

void update() {
	running = !glfwWindowShouldClose(Window::window);
	glfwPollEvents();
}

void render() {
	/*
	We need to learn how to render vertices
	*/
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	

	



	glfwSwapBuffers(Window::window);
}

void loop() {
	double time = glfwGetTime();
	while (running) {
		if (glfwGetTime() - time >= 1) { //Updates once per second for now
			update();
			render();
			time += glfwGetTime();
		}
	}
	glfwTerminate();
}

int main() {
	if (!init()) {
		std::cout << "Failed to init! :(" << std::endl;
		std::cin.get();
		return 0;
	}
	loop();
}