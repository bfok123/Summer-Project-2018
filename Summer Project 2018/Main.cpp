#include "MathStuff.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include "Window.h"

bool running;
Matrix rotate(4, 4);
/*
Vector fl1(0.0f, 0.0f, 0.0f);
Vector fl2(0.0f, -0.5f, 0.5f);
Vector fl3(-0.5f, -0.5f, 0.0f);
Vector fr1(0.0f, 0.0f, 0.0f);
Vector fr2(0.0f, -0.5f, 0.5f);
Vector fr3(0.5f, -0.5f, 0.0f);
Vector bl1(0.0f, 0.0f, 0.0f);
Vector bl2(-0.5f, -0.5f, 0.0f);
Vector bl3(0.0f, -0.5f, -0.5f);
Vector br1(0.0f, 0.0f, 0.0f);
Vector br2(0.5f, -0.5f, 0.0f);
Vector br3(0.0f, -0.5f, -0.5f);*/
Vector tl(-0.5, 0.5, 0);
Vector tr(0.5, 0.5, 0);
Vector bl(-0.5, -0.5, 0);
int width, height;

void init(int windowWidth, int windowHeight, int windowScale, const char* title) {
	glfwInit();
	Window::window = glfwCreateWindow(Window::width = Window::widthRes = windowWidth,
									  Window::height = Window::heightRes = windowHeight, 
									  Window::title = title, NULL, NULL);


	//GLFW STUFF
	glfwMakeContextCurrent(Window::window);
	
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
	/*
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Specific ID that the GPU program runs on
	glOrtho(0, width, 0, height, -10, 10); //This is camera stuff; -10 and 10 is for layering
	glDepthRange(-10, 10);
	*/

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//^^^ IDK HOW MUCH OF THIS IS RELEVANT FOR 3D
	
	glfwSetWindowSize(Window::window, windowWidth * windowScale, windowHeight * windowScale);
	
	rotate.identity();
	Matrix model(4, 4);
	//model.identity();
	//model.rotate(30.0f, 1.0f, 1.0f, 1.0f);
	Matrix view(4, 4);
	//view.identity();
	//view.translate(0.0f, 0.0f, -14.0f);
	Matrix projection(4, 4);
	//projection.perspective(90.0f, width / height, 0.1f, 100.0f);
	model.elements[0] = 0.9107;
	model.elements[1] = 0.3333;
	model.elements[2] = -0.2440;
	model.elements[3] = 0;
	model.elements[4] = -0.2440;
	model.elements[5] = 0.9107;
	model.elements[6] = 0.3333;
	model.elements[7] = 0;
	model.elements[8] = 0.3333;
	model.elements[9] = -0.2440;
	model.elements[10] = 0.9107;
	model.elements[11] = 0;
	model.elements[12] = 0;
	model.elements[13] = 0;
	model.elements[14] = 0;
	model.elements[15] = 1;
	view.identity();
	view.elements[14] = -14;
	projection.zero();
	projection.elements[0] = 1.25;
	projection.elements[5] = 1.667;
	projection.elements[10] = -1.1333;
	projection.elements[11] = -1;
	projection.elements[14] = -10.667;
	rotate = projection * view * model;
	for (int i = 0; i < 16; i++) {
		std::cout << rotate.elements[i] << " ";
	}
	/*
	fl1 = rotate * fl1;
	fl2 = rotate * fl2;
	fl3 = rotate * fl3;
	fr1 = rotate * fr1;
	fr2 = rotate * fr2;
	fr3 = rotate * fr3;
	bl1 = rotate * bl1;
	bl2 = rotate * bl2;
	bl3 = rotate * bl3;
	br1 = rotate * br1;
	br2 = rotate * br2;
	br3 = rotate * br3;
	*/
	
	running = true;
}

void update() {
	running = !glfwWindowShouldClose(Window::window);
	
	tl = rotate * tl;
	tr = rotate * tr;
	bl = rotate * bl;
}

void render() {
	/*
	We need to learn how to render vertices
	*/


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glColor3f(0.5f, 0.32f, 0.63f);
	glVertex3f(tl.elements[Coordinates::X], tl.elements[Coordinates::Y], tl.elements[Coordinates::Z]);
	glColor3f(0.26f, 0.46f, 0.92f);
	glVertex3f(tr.elements[Coordinates::X], tr.elements[Coordinates::Y], tr.elements[Coordinates::Z]);
	glColor3f(0.37f, 0.74f, 0.17f);
	glVertex3f(bl.elements[Coordinates::X], bl.elements[Coordinates::Y], bl.elements[Coordinates::Z]);

	/*
	glColor3f(0.5f, 0.75f, 0.5f);
	glVertex3f(fl1.elements[Coordinates::X], fl1.elements[Coordinates::Y], fl1.elements[Coordinates::Z]);
	glColor3f(0.23f, 1.0f, 0.29f);
	glVertex3f(fl2.elements[Coordinates::X], fl2.elements[Coordinates::Y], fl2.elements[Coordinates::Z]);
	glColor3f(0.82f, 0.72f, 0.38f);
	glVertex3f(fl3.elements[Coordinates::X], fl3.elements[Coordinates::Y], fl3.elements[Coordinates::Z]);

	glColor3f(0.3f, 0.25f, 0.73f);
	glVertex3f(fr1.elements[Coordinates::X], fr1.elements[Coordinates::Y], fr1.elements[Coordinates::Z]);
	glColor3f(0.73f, 1.0f, 0.0f);
	glVertex3f(fr2.elements[Coordinates::X], fr2.elements[Coordinates::Y], fr2.elements[Coordinates::Z]);
	glColor3f(0.82f, 0.72f, 0.38f);
	glVertex3f(fr3.elements[Coordinates::X], fr3.elements[Coordinates::Y], fr3.elements[Coordinates::Z]);

	glColor3f(0.64f, 0.234f, 0.54f);
	glVertex3f(bl1.elements[Coordinates::X], bl1.elements[Coordinates::Y], bl1.elements[Coordinates::Z]);
	glColor3f(0.13f, 0.0f, 0.83f);
	glVertex3f(bl2.elements[Coordinates::X], bl2.elements[Coordinates::Y], bl2.elements[Coordinates::Z]);
	glColor3f(0.24f, 0.63f, 0.72f);
	glVertex3f(bl3.elements[Coordinates::X], bl3.elements[Coordinates::Y], bl3.elements[Coordinates::Z]);

	glColor3f(0.23f, 0.5f, 0.92f);
	glVertex3f(br1.elements[Coordinates::X], br1.elements[Coordinates::Y], br1.elements[Coordinates::Z]);
	glColor3f(0.723f, 0.63f, 0.18f);
	glVertex3f(br2.elements[Coordinates::X], br2.elements[Coordinates::Y], br2.elements[Coordinates::Z]);
	glColor3f(0.72f, 0.36f, 0.62f);
	glVertex3f(br3.elements[Coordinates::X], br3.elements[Coordinates::Y], br3.elements[Coordinates::Z]);
	*/
	glEnd();

	glfwSwapBuffers(Window::window);
}

void loop() {
	unsigned int fpsCounter = 0;
	double time = glfwGetTime();
	double previousTime = glfwGetTime();
	double interval = 1.0 / 1.0;
	double difference = 0.0;
	while (running) {
		double currentTime = glfwGetTime();
		difference += (currentTime - previousTime) / interval;
		previousTime = currentTime;
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
}

int main() {
	int width = 800;
	int height = 450;
	int scale = 1;
	const char* title = "boop";
	init(width, height, scale, title);
	loop();
}