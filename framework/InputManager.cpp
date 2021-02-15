

#include "InputManager.h"

#include "../GameEngine3D/GameLogic.h"

void InputManager::init() {
	glfwSetKeyCallback(WindowManager::getWindow(), keyCallback);
	glfwSetCharCallback(WindowManager::getWindow(), characterCallback);
	glfwSetMouseButtonCallback(WindowManager::getWindow(), mouseButtonCallback);
	glfwSetCursorPosCallback(WindowManager::getWindow(), mouseMoveCallback);
	glfwSetScrollCallback(WindowManager::getWindow(), mouseScrollCallback);
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	GameLogic::keyPressed(key, action, mods);
}

void InputManager::characterCallback(GLFWwindow* window, unsigned int codepoint) {

}

void InputManager::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
}

void InputManager::mouseMoveCallback(GLFWwindow* window, double xPos, double yPos) {


}

void InputManager::mouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
}

std::array<float, 2> InputManager::getMousePos() {
	double x, y;
	std::array<float, 2> pos = std::array<float, 2>();
	glfwGetCursorPos(WindowManager::getWindow(), &x, &y);
	pos[0] = (float)x;
	pos[1] = (float)y;
	return pos;
}

float InputManager::getMouseX() {
	double x, y;
	glfwGetCursorPos(WindowManager::getWindow(), &x, &y);
	return (float)x;
}

float InputManager::getMouseY() {
	double x, y;
	glfwGetCursorPos(WindowManager::getWindow(), &x, &y);
	return (float)y;
}

bool InputManager::queryKeyPressed(int glfwKey) {
	int state = glfwGetKey(WindowManager::getWindow(), glfwKey);
	return state == GLFW_PRESS;
}

void InputManager::cleanup()
{
}
