

#include "WindowManager.h"

#include "OpenGLdebug.h"

#include <iostream>

#include "gtc/matrix_transform.hpp"

#include <array>
#include <time.h>

GLFWwindow* WindowManager::window = nullptr;

int WindowManager::virtualHeight;
int WindowManager::virtualWidth;
glm::mat4 WindowManager::projectionMatrix;

float WindowManager::currentFPS = 0.0f;

void WindowManager::init(std::string title, int width, int height){

    if (!glfwInit()){
        std::cerr << "GLFW did not initialize successfully" << std::endl;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_MAXIMIZED, true);
	glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_RESIZABLE, true);
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		std::cerr << "glfw window failed to be created" << std::endl;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW did not initialize correctly" << std::endl;
	}

	GLCALL(glEnable(GL_DEPTH_TEST));
	GLCALL(glEnable(GL_CULL_FACE));
	GLCALL(glFrontFace(GL_CCW));

	glEnable(GL_PROGRAM_POINT_SIZE);

	setVirtualDimentions(getWindowWidth(), getWindowHeight());

}

GLFWwindow* WindowManager::getWindow(){
    return window;
}

void WindowManager::setVirtualDimentions(int width, int height)
{

	virtualWidth = width;
	virtualHeight = height;

	int deviceWidth, deviceHeight;
	glfwGetWindowSize(window, &deviceWidth, &deviceHeight);

	// Calc ratio correction based on the two dimensions
	float correction;
	float deviceRatio = (float)deviceWidth / deviceHeight;
	float virtualRatio = virtualWidth / virtualHeight;
	float xCorrection = (float)deviceWidth / virtualWidth;
	float yCorrection = (float)deviceHeight / virtualHeight;

	if (virtualRatio < deviceRatio) {
		correction = yCorrection;
	}
	else {
		correction = xCorrection;
	}

	// Now when you calc your ortho projection--in this case centered--
	// just divide by the correction
	float left = -width / 2.0f / correction;
	float right = width / 2.0f / correction;
	float bottom = -height / 2.0f / correction;
	float top = height / 2.0f / correction;


	// Now set matrix which includes correction.
	projectionMatrix = glm::ortho(left, right, bottom, top, 0.1f, 100.0f);

	// Your viewport remains unchanged and is simply lower-left aligned
	// For example
	glViewport(0, 0, deviceWidth, deviceHeight);
}

int WindowManager::getVirtualWidth()
{
	return virtualWidth;
}

int WindowManager::getVirtualHeight()
{
	return virtualHeight;
}

void WindowManager::setWindowDimentions(int width, int height)
{
	glfwSetWindowSize(window, width, height);
	setVirtualDimentions(virtualWidth, virtualHeight);
}

int WindowManager::getWindowWidth()
{
	int width;
	glfwGetWindowSize(window, &width, nullptr);
	return width;
}

int WindowManager::getWindowHeight()
{
	int height;
	glfwGetWindowSize(window, nullptr, &height);
	return height;
}

/* width/height */
float WindowManager::getAspectRatio() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	return (float)width/height;
}

glm::mat4 WindowManager::getProjectionMatrix() {
	return glm::perspective(1.6f, (float)getWindowWidth() / (float)getWindowHeight(), 0.3f, 500.0f); 
}


float WindowManager::getCurrentFPS()
{
	return currentFPS;
}

void WindowManager::updateFrameRate() {
	static int currentIndex = 0;
	static float previousTime = 0;
	static std::array<float, 200> deltaTimes = { 0 };

	float totalDeltas = 0;
	for (int i = 0; i < deltaTimes.size(); i++)
		totalDeltas += deltaTimes[i];

	if (totalDeltas > 0)
		currentFPS = 1.0f / (totalDeltas / 200.0f);

	deltaTimes[currentIndex] = ((float)clock() / CLOCKS_PER_SEC) - previousTime;

	currentIndex++;
	if (currentIndex >= 200) currentIndex = 0;
	previousTime = ((float)clock() / CLOCKS_PER_SEC);
}

void WindowManager::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}