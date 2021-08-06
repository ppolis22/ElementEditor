#include "Camera.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

Camera::Camera() {
	projectionMatrix = glm::perspective(glm::radians(FOV), 960.0f / 540.0f, NEAR_PLANE, FAR_PLANE);

	position = glm::vec3(0.0f, 0.0, 5.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	globalUp = glm::vec3(0.0f, 1.0f, 0.0f);

	distance = 5.0f;
	yaw = -90.f;
	pitch = 0.0f;
	movementSpeed = 2.5f;
	mouseSensitivity = 0.1f;
}

Camera::~Camera() {}

glm::mat4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, target, globalUp);
}

void Camera::rotate(float deltaX, float deltaY) {
	yaw += (deltaX * mouseSensitivity);
	pitch += (deltaY * mouseSensitivity);

	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	position.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * distance;
	position.y = sin(glm::radians(pitch)) * distance;
	position.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * distance;
}