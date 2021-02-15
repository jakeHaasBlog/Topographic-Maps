#pragma once

#include "GL/glew.h"

class VertexBuffer {

private:
	GLuint rendererID;

public:
	VertexBuffer();
	VertexBuffer(const void* data, GLuint size);

	void bufferData(const void* data, GLuint size);

	inline GLuint getID() const { return rendererID; }

    void bind() const;
	void unbind() const;

};