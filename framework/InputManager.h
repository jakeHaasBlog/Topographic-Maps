#pragma once

#include "WindowManager.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <array>

class InputManager {
public:
	static void init();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void characterCallback(GLFWwindow* window, unsigned int codepoint);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static std::array<float, 2> getMousePos();
	static float getMouseX();
	static float getMouseY();

	/* returns if a ky is pressed, i.e:
		if (queryKeyPressed(GLFW_KEY_W)){
			doThing();
		}
	*/
	static bool queryKeyPressed(int glfwKey);

	static void cleanup();

private:


};