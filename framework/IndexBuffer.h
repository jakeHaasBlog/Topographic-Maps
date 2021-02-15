#pragma once

#include "GL/glew.h"

class IndexBuffer {

private:
	GLuint rendererID;
	unsigned int count;

public:
	IndexBuffer();
	IndexBuffer(void* data, unsigned int size);


	inline GLuint getID() const { return rendererID; };

	void bufferData(void* data, unsigned int size);

	unsigned int getCount() const { return count; };


	void bind() const;
	void unBind() const;

};