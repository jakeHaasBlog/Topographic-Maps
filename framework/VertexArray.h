
#pragma once

#include "GL/glew.h"
#include "VertexBuffer.h"

#include <string>


class VertexArray {
public:

    VertexArray();
	VertexArray(VertexBuffer vb, std::string format);
    ~VertexArray();

    unsigned int getID();

    /* 
		Each vertex buffer object holds one attribute 
		ie. 3f-pos, 2f-texCoord, or 1i-boneIndex 
       
		Format: std::string like "fff" for storing 3 floats or "i" for one integer 
    */
    void addVertexAttributeBuffer(const VertexBuffer& vb, std::string format);

	/*
		One vertex buffer holds all attributes
		ie. pos-normal-texcoord-boneindex

		Format: std::string like "fff fff ff" for pos(3f)-normal(3f)-texcoord(2f)
	*/
	void setIndividualVertexBuffer(VertexBuffer vb, std::string format);

    void bind() const;
    void unbind() const;

private:
    unsigned int id;
};