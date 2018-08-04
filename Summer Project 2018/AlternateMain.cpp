#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Mouse.h"
#include "Entity.h"

bool running;
Camera camera(Vector(0, 0, 0));

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 450;
const unsigned int SCALE = 3;
const char* TITLE = "boop";

bool firstMouseMovement = true;

void mouseCallback(GLFWwindow* window, double x, double y) {
	if (firstMouseMovement) {
		Mouse::x = x;
		Mouse::y = y;
		firstMouseMovement = false;
	}
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	Mouse::xOffset = x - Mouse::x;
	Mouse::yOffset = y - Mouse::y;
	Mouse::x = x;
	Mouse::y = y;

	camera.processMouseMovement(Mouse::xOffset, Mouse::yOffset);
}

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
	glfwSetMouseButtonCallback(Window::window, Mouse::mouseButtonCallback);
	glfwSetCursorPosCallback(Window::window, mouseCallback);
	glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "GLEW failed to init! :(" << std::endl;
		std::cout << glewGetErrorString(error) << std::endl;
		return false;
	}

	//GL
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST); // So things in the back don't render in front of things in the front.

	//Callbacks
	glfwSetFramebufferSizeCallback(Window::window, Window::windowSizeCallback);
	
	//Created a Shader obect
	Shader shader("Shader.vertex", "Shader.fragment");

	//Vertex Buffer Object and Element Object Buffer Setup
	//TEST VERTICES FOR TRIANGLES
	//Square
	/*
	float vertices[] = { //(x, y, z, r, g, b)
		0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f, //Top right
		0.5f, -0.5f, 0.0f,	  0.0f, 1.0f, 0.0f, //Bottom right
	   -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, //Bottom left
	   -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f  //Top left
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	*/
	float vertices[] = {
	   -0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 1.0f,

	   -0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,

	   -0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,

	   -0.5f,  0.5f, -0.5f,		0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,
	};

	Entity cubes[] = {
		Entity(0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f),
		Entity(2.0f,  5.0f, -15.0f, 1.0f, 1.0f, 1.0f),
		Entity(-1.5f, -2.2f, -2.5f, 1.0f, 1.0f, 1.0f),
		Entity(-3.8f, -2.0f, -12.3f, 1.0f, 1.0f, 1.0f),
		Entity(2.4f, -0.4f, -3.5f, 1.0f, 1.0f, 1.0f),
		Entity(-1.7f,  3.0f, -7.5f, 1.0f, 1.0f, 1.0f),
		Entity(1.3f, -2.0f, -2.5f, 1.0f, 1.0f, 1.0f),
		Entity(1.5f,  2.0f, -2.5f, 1.0f, 1.0f, 1.0f),
		Entity(1.5f,  0.2f, -1.5f, 1.0f, 1.0f, 1.0f),
		Entity(-1.3f,  1.0f, -1.5f, 1.0f, 1.0f, 1.0f),
		//Entity(camera.position.elements[Coordinates::X], camera.position.elements[Coordinates::Y], camera.position.elements[Coordinates::Z] + camera.front.elements[Coordinates::Z], 5.0f, 5.0f, 5.0f)
	};

	unsigned int VBO, VAO/*, EBO*/;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0); //0, because layout location for position is 0
					  //ID  elements ... ... Stride of 6 * sizeof(float)     offset
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1); //1, because layout location for color is 1

	running = true;

	float x = 0.0f;
	float y = -3.0f;

	while (!glfwWindowShouldClose(Window::window)) {
		glfwPollEvents();
		Matrix view(4, 4);

		if (glfwGetKey(Window::window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.processKeyboard(CameraMovement::LEFT, 0.3f);
		if (glfwGetKey(Window::window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.processKeyboard(CameraMovement::RIGHT, 0.3f);
		if (glfwGetKey(Window::window, GLFW_KEY_UP) == GLFW_PRESS)    camera.processKeyboard(CameraMovement::FORWARD, 0.3f);
		if (glfwGetKey(Window::window, GLFW_KEY_DOWN) == GLFW_PRESS)  camera.processKeyboard(CameraMovement::BACKWARD, 0.3f);
		
		//cubes[10].x = camera.position.elements[Coordinates::X] + camera.front.elements[Coordinates::X];
		//cubes[10].y = camera.position.elements[Coordinates::Y] + camera.front.elements[Coordinates::Y];
		//cubes[10].z = camera.position.elements[Coordinates::Z] + camera.front.elements[Coordinates::Z];
		
		/*
		std::cout << "camera: ";
		for (int i = 0; i < 3; i++) {
			std::cout << camera.position.elements[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "cube: ";
		std::cout << cubes[10].x << " " << cubes[10].y << " " << cubes[10].z;
		std::cout << std::endl;*/

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		Matrix model(4, 4);
		Matrix projection(4, 4);


		Vector i(1.0f, 0.0f, 0.0f);
		Vector j(0.0f, 1.0f, 0.0f);
		Vector k(0.0f, 0.0f, 1.0f);
		Vector ij(1.0f, 1.0f, 0.0f);
		Vector u(0.5f, 0.0f, 0.0f);
		model.identity();
		
#define THIRD
#ifdef FIRST
		// TEST ONE (only component i (1, 0, 0)):             PASSED
		model.rotate(glfwGetTime() * 50.0f, i);
#endif
#ifdef SECOND
		// TEST TWO (only component j (0, 1, 0)):			  PASSED
		model.rotate(glfwGetTime() * 50.0f, j);
#endif
#ifdef THIRD
		// TEST THREE (component i then j separately):        PASSED
		//model.rotate(glfwGetTime() * 50.0f, i);
		//model.rotate(glfwGetTime() * 50.0f, j);
#endif
#ifdef FOURTH
		// TEST FOUR (component j then i separately):         FAILED
		model.rotate(glfwGetTime() * 50.0f, j);				//- wrong rotation
		model.rotate(glfwGetTime() * 50.0f, i);				//+ didn't deform/stretch
		for (int i = 0; i < 16; i++) {
			std::cout << model.elements[i] << " ";
		}
		std::cout << std::endl;
#endif
#ifdef FIFTH
		// TEST FIVE (multiple components at the same time):  FAILED
		model.rotate(glfwGetTime() * 50.0f, ij);			//- wrong rotation
															//- did deform/stretch
#endif
#ifdef SIXTH
		// TEST SIX (only component u (0.5, 0, 0)):           FAILED
		model.rotate(glfwGetTime() * 50.0f, u);				//+ correct rotation
															//- did deform/stretch
#endif




		// Vector b(0.0f, 0.0f, -3.0f);
		// Vector b(sin(glfwGetTime()), cos(glfwGetTime()), -3.0f);
		// Vector b(0.0f, 0.0f, -3.0f);
		Vector b(0.0f, 0.0f, y);
		
		// camera/view transformation
		/*float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = lookAt(Vector(camX, 0.0f, camZ), Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
		view.translate(b);
		for (int i = 0; i < 4; i++) {
			std::cout << view.elements[i] << " ";
		}
		std::cout << std::endl;*/

							// fov    aspect                          near  far (render distance)
		projection.perspective(45.0f, (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
		shader.setUniformMatrix4("projection", projection);
		view = camera.getViewMatrix();
		shader.setUniformMatrix4("view", view);

		glBindVertexArray(VAO);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		for (unsigned int i = 0; i < 10; i++) {
			Matrix model(4, 4);
			model.identity();
			model.translate(cubes[i].getPositionVector());
			//float angle = 20.0f * i;
			//model.rotate(angle, ij);
			shader.setUniformMatrix4("model", model);
			if (i != 0)
				glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glfwSwapBuffers(Window::window);
	}


	return running = true;
}

void update() {
	running = !glfwWindowShouldClose(Window::window);
	glfwPollEvents();
}

void render() {
	
	// We need to learn how to render vertices
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	





	glfwSwapBuffers(Window::window);
}

void loop() {
	unsigned int fpsCounter = 0;

	double time = glfwGetTime();
	double previousTime = glfwGetTime();
	double interval = 1.0 / 1.0;
	double difference = 0.0;
	
	while (running) {
		while (difference >= 1) {
			update();
			difference--;
		}
		render();
		fpsCounter++;
		if (glfwGetTime() - time >= 1) {
			time += 1.0;
			//Window::setTitle((Window::title + std::string(" | ") + std::to_string(fpsCounter) + " FPS").c_str());
			fpsCounter = 0;
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