#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

class MousePicker {
public:
	MousePicker(int windowWidth, int windowHeight, glm::mat4 projectionMatrix);
	~MousePicker();

	glm::vec3 getRayFromScreenCoords(const glm::mat4& viewMatrix, float mousePosX, float mousePosY);

private:
	int windowWidth;
	int windowHeight;
	glm::mat4 projectionMatrix;
};