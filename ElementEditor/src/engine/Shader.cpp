#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	:id(0) {

	ShaderProgramSource source = { parseFile(vertexPath), parseFile(fragmentPath) };
	id = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {}

void Shader::bind() const {
	glUseProgram(id);
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformVec3f(const std::string& name, const glm::vec3& vector) {
	glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::setUniformVec4f(const std::string& name, const glm::vec4& vector) {
	glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::setUniformFloat(const std::string& name, const float value) {
	glUniform1f(getUniformLocation(name), value);
}

std::string Shader::parseFile(const std::string& path) {
	std::ifstream shaderFile(path);
	std::stringstream shaderStream;

	shaderStream << shaderFile.rdbuf();
	shaderFile.close();

	return shaderStream.str();
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int shaderId = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(shaderId);
		return 0;
	}

	return shaderId;
}

unsigned int Shader::getUniformLocation(const std::string& name) {
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
		return uniformLocationCache[name];
	}

	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		std::cout << "Warning uniform '" << name << "' doesn't exist!" << std::endl;
	}

	uniformLocationCache[name] = location;
	return location;
}