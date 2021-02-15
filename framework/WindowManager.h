#pragma once


#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"

#include <string>

class WindowManager {
public:
    static void init(std::string title, int width, int height);

    static GLFWwindow* getWindow();

	static void setVirtualDimentions(int dimX, int dimY);
	static int getVirtualWidth();
	static int getVirtualHeight();

	static void setWindowDimentions(int width, int height);
	static int getWindowWidth();
	static int getWindowHeight();

	static float getAspectRatio();

	static glm::mat4 getProjectionMatrix();

	static float getCurrentFPS();
	static void updateFrameRate();

	static void cleanup();

private:
    static GLFWwindow* window;

	static int virtualHeight;
	static int virtualWidth;
	static glm::mat4 projectionMatrix;

	static float currentFPS;
};
