#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

class MousePicker {
public:
	MousePicker(int windowWidth, int windowHeight, glm::mat4& projectionMatrix);
	~MousePicker();

	glm::vec3 getCurrentRay(glm::mat4& viewMatrix, float mousePosX, float mousePosY);

private:
	int windowWidth;
	int windowHeight;
	glm::mat4 projectionMatrix;

	glm::vec2 getNormalizedDeviceCoords(float mouseX, float mouseY);
	glm::vec4 getEyeSpaceCoords(glm::vec4& clipSpaceCoords);
	glm::vec3 getWorldSpaceRay(glm::vec4& eyeSpaceCoords, glm::mat4& viewMatrix);
};