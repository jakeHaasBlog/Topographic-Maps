
#include "VertexBuffer.h"

#include "OpenGLdebug.h"

VertexBuffer::VertexBuffer() {
	GLCALL(glGenBuffers(1, &rendererID));
}

VertexBuffer::VertexBuffer(const void* data, GLuint size) :
	VertexBuffer()
{
	bufferData(data, size);
}

void VertexBuffer::bufferData(const void* data, GLuint size) {
	bind();
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::bind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::unbind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

