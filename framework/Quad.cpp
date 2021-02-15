#include "Quad.h"

Quad::Quad() {
	vb = new VertexBuffer();
	vb->bufferData(vertices, 4 * 2 * sizeof(float));

	ib = new IndexBuffer();
	ib->bufferData(indices, 6 * sizeof(int));

	va = new VertexArray();
	va->setIndividualVertexBuffer(*vb, "ff");

}

void Quad::setPos(float x, float y) {
	posDim[0] = x;
	posDim[1] = y;
}

void Quad::setDim(float width, float height) {
	posDim[2] = width;
	posDim[3] = height;
}


void Quad::render(Shader* shader) {

	shader->setUniform4f("u_posDim", posDim);
	shader->bind();

	va->bind();
	vb->bind();
	ib->bind();

	glDisable(GL_CULL_FACE);
	glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);
	glEnable(GL_CULL_FACE);
}