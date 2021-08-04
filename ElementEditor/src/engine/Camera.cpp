#include "Camera.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

Camera::Camera() {
	projectionMatrix = glm::perspective(glm::radians(FOV), 960.0f / 540.0f, NEAR_PLANE, FAR_PLANE);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
}

Camera::~Camera() {}

glm::mat4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
}