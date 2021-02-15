#pragma once


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

class Quad {
public:
	Quad();

	void setPos(float x, float y);
	void setDim(float width, float height);

	void render(Shader* shader);

private:
	float vertices[4 * 2] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	}; // x, y
	int indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	float posDim[4] = { 0, 0, 0.5, 0.5 };

	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;
	VertexArray* va = nullptr;

};