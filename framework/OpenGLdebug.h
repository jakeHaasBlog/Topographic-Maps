#pragma once

#include "GL/glew.h"
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) OpenGLdebug::GLClearError();\
	x;\
	ASSERT(OpenGLdebug::GLLogCall(#x, __FILE__, __LINE__));

namespace OpenGLdebug {
	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
	void GLCheckError();
}