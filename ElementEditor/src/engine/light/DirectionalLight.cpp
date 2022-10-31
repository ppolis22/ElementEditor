#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(int xBound, int yBound, int zBound, float yAngle, float xAngle, glm::vec3 color) 
	: color(color), xBound(xBound), yBound(yBound), zBound(zBound), yAngle(yAngle), xAngle(xAngle)
{
	shadowMapCastData = computeShadowMapCastData(yAngle, xAngle, xBound, zBound, yBound);
}

DirectionalLight::ShadowMapCastData DirectionalLight::computeShadowMapCastData(
	float yAngle, float xAngle, int projectWidth, int projectDepth, int projectHeight
) {
	// compute the width of the bounding box
	float projectionWidth = projectWidth * abs(cos(yAngle)) + projectDepth * abs(sin(yAngle));
	float tempDepth = projectWidth * abs(sin(yAngle)) + projectDepth * abs(cos(yAngle));

	// compute the depth and height of the bounding box
	float projectionDepth = tempDepth * abs(cos(xAngle)) + projectHeight * abs(sin(xAngle));
	float projectionHeight = tempDepth * abs(sin(xAngle)) + projectHeight * abs(cos(xAngle));

	glm::mat4 shadowMapProjectionMat = glm::ortho(		// left, right, down, up, near, far
		-projectionWidth / 2.0f, projectionWidth / 2.0f,
		-projectionHeight / 2.0f, projectionHeight / 2.0f,
		0.01f, projectionDepth);

	// define the "target" of the projection vector as the center of the project
	glm::vec3 target(projectWidth / 2.0f, projectHeight / 2.0f, projectDepth / 2.0f);

	// compute the position for the projection
	glm::vec3 position(0.0f, 0.0f, projectionDepth / 2.0f);
	position = glm::rotate(glm::mat4(1.0f), yAngle, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(position, 0.0f);
	position = glm::rotate(glm::mat4(1.0f), xAngle, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(position, 0.0f);
	position = glm::translate(glm::mat4(1.0f), target) * glm::vec4(position, 1.0f);

	glm::vec3 direction = glm::normalize(target - position);

	glm::mat4 viewMatrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));

	return ShadowMapCastData{ shadowMapProjectionMat, viewMatrix, direction };
}

glm::vec3 DirectionalLight::getColor() {
	return color;
}

glm::mat4 DirectionalLight::getViewMatrix() {
	return shadowMapCastData.viewMatrix;
}

glm::mat4 DirectionalLight::getProjectionMatrix() {
	return shadowMapCastData.projectionMatrix;
}

glm::vec3 DirectionalLight::getDirectionVector() {
	return shadowMapCastData.directionVector;
}