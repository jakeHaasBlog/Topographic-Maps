
#include "OpenGLdebug.h"


void OpenGLdebug::GLClearError() {
	while (glGetError());
}

bool OpenGLdebug::GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL error]: " << function << " in " << file << "   GL CODE: " << error << std::endl;
		return false;
	}
	return true;
}

void OpenGLdebug::GLCheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL error]: " << error << std::endl;
	}
}