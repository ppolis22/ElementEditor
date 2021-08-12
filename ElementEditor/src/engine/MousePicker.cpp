#include "MousePicker.h"

MousePicker::MousePicker(int windowWidth, int windowHeight, glm::mat4& projectionMatrix) 
	: windowWidth(windowWidth), windowHeight(windowHeight), projectionMatrix(projectionMatrix) {}

MousePicker::~MousePicker() {}

glm::vec3 MousePicker::getCurrentRay(glm::mat4& viewMatrix, float mousePosX, float mousePosY) {
	glm::vec2 normalizedCoords = getNormalizedDeviceCoords(mousePosX, mousePosY);

	// w must be 1 if x and y are perspective-divided coords, I think z can be anything? Test this
	glm::vec4 clipCoords(normalizedCoords.x, normalizedCoords.y, -1.0, 1.0);

	glm::vec4 eyeCoords = getEyeSpaceCoords(clipCoords);
	glm::vec3 worldRay = getWorldSpaceRay(eyeCoords, viewMatrix);
	return worldRay;
}

glm::vec2 MousePicker::getNormalizedDeviceCoords(float mouseX, float mouseY) {
	float normalizedX = (2.0f * mouseX) / windowWidth - 1.0f;
	float normalizedY = 1.0f - (2.0f * mouseY) / windowHeight;	// mouse Y comes in inverted, this corrects for that
	return glm::vec2(normalizedX, normalizedY);
}

glm::vec4 MousePicker::getEyeSpaceCoords(glm::vec4& clipSpaceCoords) {
	glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipSpaceCoords;
	// here z is hardcoded to -1, but I think that only works if clip space w coord is also hardcoded to 1
	// w set to 0 so this represents a vector rather than a point
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0, 0.0);
}

glm::vec3 MousePicker::getWorldSpaceRay(glm::vec4& eyeSpaceCoords, glm::mat4& viewMatrix) {
	glm::vec4 worldRay = glm::inverse(viewMatrix) * eyeSpaceCoords;
	glm::vec3 normalizedWorldRay = glm::normalize(glm::vec3(worldRay.x, worldRay.y, worldRay.z));
	return normalizedWorldRay;
}