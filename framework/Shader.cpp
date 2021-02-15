#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "OpenGLdebug.h"

Shader::Shader(const std::string& _filepath)
	: filePath(_filepath), rendererID(0)
{
	ShaderProgramSource source = parseShader(_filepath);
	rendererID = createShader(source.vertexSource, source.geometrySource, source.fragmentSource);
}


Shader::~Shader()
{
	GLCALL(glDeleteProgram(rendererID));
	std::cout << "Deleted shader: " << rendererID << std::endl;
}


ShaderProgramSource Shader::parseShader(const std::string& path) {

	std::ifstream stream(path);
	if (!stream) {
		std::cout << "Did not find file at " << "sln/" << path << std::endl;
	}

	enum class ShaderType {
		NONE = -1, VERTEX = 0, GEOMETRY = 1, FRAGMENT = 2
	};

	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {

			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("geometry") != std::string::npos) {
				type = ShaderType::GEOMETRY;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
			std::cout << line << std::endl;
		}
	}

	return { ss[0].str(), ss[1].str(), ss[2].str() };

}


unsigned int Shader::compileShader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex shader" : GL_GEOMETRY_SHADER ? "geometry shader" : "fragment shader")
			<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		delete[] message;
		return 0;
	}

	return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, gs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);

	return program;
}

void Shader::bind() const {
	GLCALL(glUseProgram(rendererID));
}

void Shader::unbind() const {
	GLCALL(glUseProgram(0));
}

void Shader::setUniform3f(const std::string& name, const float& v0, const float& v1, const float& v2) {
	bind();
	GLCALL(glUniform3f(getUniformLocation(name), v0, v1, v2));
	unbind();
}

void Shader::setUniform4f(const std::string& name, const float& v0, const float& v1, const float& v2, const float& v3) {
	bind();
	GLCALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
	unbind();
}


void Shader::setUniform3f(const std::string& name, const float values[3]) {
	bind();
	GLCALL(glUniform3f(getUniformLocation(name), values[0], values[1], values[2]));
	unbind();
}

void Shader::setUniform4f(const std::string& name, const float values[4]) {
	bind();
	GLCALL(glUniform4f(getUniformLocation(name), values[0], values[1], values[2], values[3]));
	unbind();
}

void Shader::setUniform1i(const std::string& name, const int& value) {
	bind();
	GLCALL(glUniform1i(getUniformLocation(name), value));
	unbind();
}

void Shader::setUniform1f(const std::string& name, const float& value) {
	bind();
	GLCALL(glUniform1f(getUniformLocation(name), value));
	unbind();
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	bind();
	GLCALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	unbind();
}

void Shader::setUniformArrayFloat(const std::string& name, const float* data, const unsigned int& size) {
	bind();
	glUniform1fv(getUniformLocation(name), size / sizeof(float), data);
	unbind();
}

void Shader::setUniformArrayInt(const std::string& name, const int* data, const unsigned int& size) {
	bind();
	glUniform1iv(getUniformLocation(name), size / sizeof(int), data);
	unbind();
}

void Shader::setUniformArrayFloat3(const std::string& name, const float* data, const unsigned int& size) {
	bind();
	GLCALL(glUniform3fv(getUniformLocation(name), size / (3 * sizeof(float)), data));
	unbind();
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
		return uniformLocationCache[name];
	}

	GLCALL(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform " << name << " doesn't exist" << std::endl;
	}

	uniformLocationCache[name] = location;
	return location;
}
