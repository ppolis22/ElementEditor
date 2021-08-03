#pragma once

#include "../vendor/glm/glm.hpp"

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void bind() const;
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	unsigned int id;
	std::unordered_map<std::string, unsigned int> uniformLocationCache;

	std::string parseFile(const std::string& path);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int getUniformLocation(const std::string& name);
};