#include "GLFW\glfw3.h"

struct Mouse {
	static void mousePosCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static inline bool button(int index) { return buttons[index]; }
	static inline int getX() { return x; }
	static inline int getY() { return y; }
	static inline int getXOffset() { return xOffset; }
	static inline int getYOffset() { return yOffset; }
	static void zeroOffsets();
//private:
	static double x, y;
	static bool buttons[];
	static double xOffset, yOffset;
	static bool firstMouseMovement;
	//static bool buttonsDown[];
	//static bool buttonsUp[];
};
