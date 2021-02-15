

#include "IndexBuffer.h"

#include "OpenGLdebug.h"

IndexBuffer::IndexBuffer()
{
	GLCALL(glGenBuffers(1, &rendererID));
	count = 0;
}

IndexBuffer::IndexBuffer(void* data, unsigned int size) :
	IndexBuffer()
{
	bufferData(data, size);
}

void IndexBuffer::bufferData(void* data, unsigned int size) {
	bind();
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	count = size / sizeof(unsigned int);
}

void IndexBuffer::bind() const {
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unBind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}