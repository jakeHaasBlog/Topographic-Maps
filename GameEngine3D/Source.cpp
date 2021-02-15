


/**********************************************************************************************************/
/**********************************************************************************************************/

//  look in GameLogic.cpp for the code you probably care about ;)

/**********************************************************************************************************/
/**********************************************************************************************************/





#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "WindowManager.h"
#include "InputManager.h"

#include <stdio.h>
#include "GameLogic.h"

void initialize();
void update();
void render();
void cleanup();

int main(int argc, const char* argv[]) {


	initialize();

	int frame = 0;
	while (!glfwWindowShouldClose(WindowManager::getWindow())) {
		glfwSwapBuffers(WindowManager::getWindow());
		glfwPollEvents();

		update();
		render();

		frame++;

		glfwMakeContextCurrent(WindowManager::getWindow());
	}

	cleanup();

}


void initialize() {
	WindowManager::init("Topographic Map Generator", 1920, 1080);
	InputManager::init();

	glfwSwapBuffers(WindowManager::getWindow());
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwMakeContextCurrent(WindowManager::getWindow());

	GameLogic::init();

}

void update() {
	GameLogic::update();

	WindowManager::updateFrameRate();

	static int frame = 0;
	frame++;

	int width, height;
	glfwGetWindowSize(WindowManager::getWindow(), &width, &height);

	double mousePos[2];
	glfwGetCursorPos(WindowManager::getWindow(), mousePos, mousePos + 1);
	int monW, monH;
	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &monW, &monH);


	float ratio = (float)width / height;
	float vWidth, vHeight, vLeft, vBottom;
	if (ratio <= 2.0f)
	{
		vWidth = width;
		vHeight = width / 2;
		vLeft = 0;
		vBottom = height / 2 - width / 4;
	}
	else
	{
		vWidth = height * 2;
		vHeight = height;
		vLeft = width / 2 - height;
		vBottom = 0;
	}

	glViewport(vLeft, vBottom, vWidth, vHeight);
	GameLogic::update();
}

void render() {
	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthFunc(GL_LEQUAL);

	GameLogic::render();
}


void cleanup() {
	WindowManager::cleanup();
	GameLogic::cleanup();
}