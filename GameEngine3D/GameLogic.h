#pragma once

#include <vector>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "PerlinNoise.hpp"

class GameLogic {
public:

	static void init();
	static void update();
	static void render();
	static void cleanup();

	static void keyPressed(int key, int action, int mods);

	static glm::mat4 viewMatrix;
	static glm::mat4 projMatrix;

	static GLuint tex;

	static double perlinNoise(double x, double y);

private:
	static std::vector<std::array<float, 3>> pixels;
	static void loadTexturePixelsThreaded();
	static void loadTexturePixelsPart(int l, int r, int t, int b);

	static std::vector<double> randomNumbers;
	static void loadRandomNumbers();
	static void loadRandomNumbersPart(int lIndex, int rIndex, siv::PerlinNoise* perlin);

	static void fillTexture();

	static unsigned int PIX_X, PIX_Y, SOFTNESS_PIX;
	static float waterLevel;
	static double octaves;
	static double frequency;
	static unsigned int posterizationLevels;
};