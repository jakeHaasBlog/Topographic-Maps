

#include "GameLogic.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "InputManager.h"
#include "Geometry.h"
#include "Quad.h"

#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <time.h>
#include <thread>

#include "PerlinNoise.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

glm::mat4 GameLogic::viewMatrix = glm::mat4(1.0f);
glm::mat4 GameLogic::projMatrix = glm::ortho(-2.0f, 2.0f, -1.0f, 1.0f, 0.1f, 100.0f);

GLuint GameLogic::tex = 0;

unsigned int GameLogic::PIX_X = 1920;
unsigned int GameLogic::PIX_Y = 1080; 
unsigned int GameLogic::SOFTNESS_PIX = 50;

std::vector<double> GameLogic::randomNumbers((PIX_X + SOFTNESS_PIX) * (PIX_Y + SOFTNESS_PIX));

std::vector<std::array<float, 3>> GameLogic::pixels(PIX_X * PIX_Y);

void GameLogic::init()
{

	loadRandomNumbers();

	std::vector<std::array<float, 3>> pixels(PIX_X * PIX_Y);
	loadTexturePixelsThreaded();

	glGenTextures(1, &tex);
	fillTexture();

}


void GameLogic::update()
{
}

void GameLogic::render()
{
	glClearColor(32 / 255.0f, 178 / 355.0f, 170 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	static Shader shader = Shader("../assets/textShader.sh");
	static float texPosDim[4] = { 0, 0, 1, 1 };
	shader.setUniform4f("u_texPosDim", texPosDim);
	shader.setUniform1i("u_texture", 0);
	static Quad q;
	q.setDim(2, 2);
	q.setPos(-1, -1);

	q.render(&shader);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void GameLogic::cleanup()
{
}


void GameLogic::keyPressed(int key, int action, int mods)
{
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {

		time_t start = clock();

		loadRandomNumbers();
		loadTexturePixelsThreaded();
		fillTexture();

		time_t end = clock();

		std::cout << "Generation Completed (" << (double)(end - start) << "ms total)" << std::endl;

	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {

		uint8_t* pixels_int = new uint8_t[PIX_X * PIX_Y * 3];
		int i = 0;
		for (std::array<float, 3>& p : pixels) {
			p[0] = std::clamp(p[0], 0.0f, 1.0f);
			p[1] = std::clamp(p[1], 0.0f, 1.0f);
			p[2] = std::clamp(p[2], 0.0f, 1.0f);

			uint8_t r = (uint8_t)(p[0] * 254.0f);
			uint8_t g = (uint8_t)(p[1] * 254.0f);
			uint8_t b = (uint8_t)(p[2] * 254.0f);

			pixels_int[i + 0] = r;
			pixels_int[i + 1] = g;
			pixels_int[i + 2] = b;

			i += 3;
		}
		stbi_write_png("../assets/stbpng.png", PIX_X, PIX_Y, 3, pixels_int, 3 * PIX_X);
		stbi_write_jpg("../assets/stbjpg.jpg", PIX_X, PIX_Y, 3, pixels_int, 100);
		delete[] pixels_int;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {

		std::cout << "\n\n\nPlease enter your generation parameters:\n";
		std::cout << "Pixels Wide (default 1920): ";
		std::cin >> PIX_X;
		std::cout << "Pixels High (default 1080): ";
		std::cin >> PIX_Y;
		std::cout << "Smoothing (default 50): ";
		std::cin >> SOFTNESS_PIX;

		randomNumbers = std::vector<double>((PIX_X + SOFTNESS_PIX) * (PIX_Y + SOFTNESS_PIX));
		pixels = std::vector<std::array<float, 3>>(PIX_X * PIX_Y);


		time_t start = clock();

		loadRandomNumbers();
		loadTexturePixelsThreaded();
		fillTexture();

		time_t end = clock();

		std::cout << "Generation Completed (" << (double)(end - start) << "ms total)" << std::endl;


	}
}


void GameLogic::loadTexturePixelsThreaded()
{
	time_t start = clock();

	std::vector<std::thread> threads;
	int threadMaxChunkSize = 1920 / 8;

	for (int x = 0; x < PIX_X; x += threadMaxChunkSize) {
		for (int y = 0; y < PIX_Y; y += threadMaxChunkSize) {

			int left = x;
			int right;
			if (x + threadMaxChunkSize > PIX_X) right = PIX_X;
			else right = x + threadMaxChunkSize;

			int top = y;
			int bottom;
			if (y + threadMaxChunkSize > PIX_Y) bottom = PIX_Y;
			else bottom = y + threadMaxChunkSize;

			threads.push_back(std::thread(loadTexturePixelsPart, left, right, top, bottom));

		}
	}

	for (std::thread& t : threads) t.join();

	time_t end = clock();
	printf("first layer of pixels took: %d ms using %d threads\n", (int)(end - start), threads.size());


	time_t start2 = clock();

	for (int i = 0; i < pixels.size(); i++) {
		int sampleArea = 3;
		float highest = 0, lowest = 1;

		for (int j = 0; j < sampleArea; j++) {
			for (int k = 0; k < sampleArea; k++) {
				int index = i + j + (k * PIX_X);
				index = std::clamp(index, 0, (int)pixels.size() - 1);
				float pAverage = (pixels[index][0] + pixels[index][1] + pixels[index][2]) / 3.0f;
				highest = std::max(pAverage, highest);
				lowest = std::min(pAverage, lowest);
			}
		}

		if (highest - lowest > 0.01f) {
			pixels[i] = { 0, 0, 0 };
		}
	}

	time_t end2 = clock();
	printf("adding lines around edges took: %d ms\n", (int)(end2 - start2));

}

void GameLogic::loadTexturePixelsPart(int l, int r, int t, int b)
{

	for (int x = l; x < r; x++) {
		for (int y = t; y < b; y++) {

			float noiseLevel = 0;
			int noiseAdditions = 0;
			float xf = (float)x, yf = (float)y;
			for (int i = 0; i < SOFTNESS_PIX; i += 6) {
				for (int j = 0; j < SOFTNESS_PIX; j += 6) {
					noiseLevel += perlinNoise(xf + i, yf + j);
					noiseAdditions++;
				}
			}
			noiseLevel /= noiseAdditions;

			float contrast = 1.5f;
			float brightness = 0.0f;
			noiseLevel = contrast * (noiseLevel - 0.5f) + 0.5f + brightness;

			int topoLevels = 20;
			noiseLevel = (int)(noiseLevel * topoLevels) / (float)topoLevels;

			float waterLevel = 0.5f;
			if (noiseLevel > waterLevel) {
				// land
				noiseLevel -= waterLevel;
				noiseLevel /= 1.0f - waterLevel;

				float landContrast = 1.0f;
				float landBrightness = 0.0f;
				noiseLevel = landContrast * (noiseLevel - 0.5f) + 0.5f + landBrightness;
				pixels[x + y * PIX_X] = { noiseLevel / 10, noiseLevel, noiseLevel / 10 };
			}
			else {
				// water
				noiseLevel /= waterLevel;

				float waterContrast = 1.0f;
				float waterBrightness = 0.0f;
				noiseLevel = waterContrast * (noiseLevel - 0.5f) + 0.5f + waterBrightness;
				pixels[x + y * PIX_X] = { noiseLevel / 10, noiseLevel / 10, noiseLevel };
			}

		}
	}

}


void GameLogic::fillTexture()
{
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PIX_X, PIX_Y, 0, GL_RGB, GL_FLOAT, &pixels[0]);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void GameLogic::loadRandomNumbers()
{
	srand(time(NULL));

	std::mt19937 generator;
	siv::PerlinNoise perlinA(generator);
	perlinA.reseed(time(NULL));
	

	std::vector<std::thread> threads;

	time_t start = clock();
	
	int threadVolume = 60000;
	for (int i = 0; i < randomNumbers.size(); i += threadVolume) {

		int lIndex = i;
		int rIndex;
		if (lIndex + threadVolume > randomNumbers.size()) rIndex = randomNumbers.size();
		else rIndex = lIndex + threadVolume;

		threads.push_back(std::thread(loadRandomNumbersPart, lIndex, rIndex, &perlinA));

	}

	for (std::thread& t : threads) t.join();

	time_t end = clock();
	printf("loading random numbers took: %d ms using %d threads\n", (int)(end - start), threads.size());

}

void GameLogic::loadRandomNumbersPart(int lIndex, int rIndex, siv::PerlinNoise* perlin)
{
	static double octaves = 10;
	static double frequency = 800;

	for (int i = 0; i < rIndex - lIndex; i++) {
		int x = (i + lIndex) % (PIX_X + SOFTNESS_PIX);
		int y = (i + lIndex) / (PIX_Y + SOFTNESS_PIX);
		randomNumbers[lIndex + i] = perlin->accumulatedOctaveNoise2D_0_1((double)x / frequency, (double)y / frequency, octaves);
	}
}


double GameLogic::perlinNoise(double x, double y)
{
	int index = x + y * (PIX_X + SOFTNESS_PIX);
	if (index < 0) index = 0;
	if (index > randomNumbers.size()) index = 0;
	return randomNumbers[index];
}