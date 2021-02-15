

#include "VertexArray.h"
#include <iostream>


VertexArray::VertexArray(){
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
}


VertexArray::VertexArray(VertexBuffer vb, std::string format) :
	VertexArray()
{
	setIndividualVertexBuffer(vb, format);
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &id);
}

unsigned int VertexArray::getID(){
    return id;
}

void VertexArray::addVertexAttributeBuffer(const VertexBuffer& vb, std::string format){

	static int attribsCount = 0;

    bind();
    vb.bind();
    int stride = 0;
    for (char c: format){
        switch (c) {
            case 'f':
                stride += sizeof(float);
            break;
            case 'i':
                stride += sizeof(int);
            break;
            default:
                std::cerr << "Type not supported" << std::endl;
        }
    }

    glEnableVertexAttribArray(attribsCount);

    if (format.at(0) == 'f')
        glVertexAttribPointer(attribsCount, format.length(), GL_FLOAT, false, stride, (const void*)0);
    else if (format.at(0) == 'i')
        glVertexAttribPointer(attribsCount, format.length(), GL_INT, false, stride, (const void*)0);

    attribsCount++;
}

void VertexArray::setIndividualVertexBuffer(VertexBuffer vb, std::string format)
{
	bind();
	vb.bind();

	format += ' ';

	int counter = 0;
	int currentAttributeIndex = 0;
	int totalOffset = 0;
	std::string currentAttribFormat = "";
	int totalStride = 0;
	for (char c : format) {
		if (c != ' ') {
			if (c == 'f') totalStride += sizeof(float);
			else std::cerr << "type not supported" << std::endl;
		}
	}
	for (char c : format) {
		counter++;
		if (c == ' ') {
			glEnableVertexAttribArray(currentAttributeIndex);

			if (currentAttribFormat.at(0) == 'f') {
				glVertexAttribPointer
				(
					currentAttributeIndex,
					currentAttribFormat.length(),
					GL_FLOAT,
					false,
					totalStride,
					(const void*)(totalOffset)
				);

				totalOffset += currentAttribFormat.length() * sizeof(float);
			}
			else {
				std::cerr << "type not supported" << std::endl;
			}

			currentAttributeIndex++;
			currentAttribFormat = "";
		}
		else {
			currentAttribFormat += c;
		}
	}

	std::cout << "";
}

void VertexArray::bind() const {
    glBindVertexArray(id);
}
void VertexArray::unbind() const {
    glBindVertexArray(0);
}