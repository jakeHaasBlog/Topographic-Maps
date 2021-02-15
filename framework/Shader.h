#pragma once

#include <string>
#include <unordered_map>

#include "glm.hpp"

struct ShaderProgramSource {
	std::string vertexSource;
	std::string geometrySource;
	std::string fragmentSource;
};

class Shader
{

private:
	std::string filePath;
	unsigned int rendererID;

	std::unordered_map<std::string, int> uniformLocationCache;

	// caching for uniforms

public:
	Shader(const std::string& filename);
	~Shader();

	int getID() const {
		return rendererID;
	}

	void bind() const;
	void unbind() const;

	// set uniforms
	void setUniform3f(const std::string& name, const float& v0, const float& v1, const float& v2);
	void setUniform4f(const std::string& name, const float& v0, const float& v1, const float& v2, const float& v3);
	void setUniform1i(const std::string& name, const int& value);
	void setUniform1f(const std::string& name, const float& value);

	void setUniform3f(const std::string& name, const float values[3]);
	void setUniform4f(const std::string& name, const float values[4]);

	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	void setUniformArrayFloat(const std::string& name, const float* data, const unsigned int& size);
	void setUniformArrayInt(const std::string& name, const int* data, const unsigned int& size);

	void setUniformArrayFloat3(const std::string& name, const float* data, const unsigned int& size);

	/*
	template<class TYPE>
	void setUniformArray(const std::string& name, const TYPE* loc, unsigned int size);
	*/

private:
	ShaderProgramSource parseShader(const std::string& file);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);
	unsigned int getUniformLocation(const std::string& name);

};

