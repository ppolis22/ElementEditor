#include "Camera.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

Camera::Camera() {
	projectionMatrix = glm::perspective(glm::radians(FOV), 960.0f / 540.0f, NEAR_PLANE, FAR_PLANE);
	position = glm::vec3(0.0f, 0.0, 5.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	localUp = glm::vec3(0.0f, 1.0f, 0.0f);
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
	return glm::lookAt(position, position + front, localUp);
}

void Camera::processMovement(float deltaX, float deltaY) {
	yaw += (deltaX * mouseSensitivity);
	pitch += (deltaY * mouseSensitivity);

	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

	// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
	right = glm::normalize(glm::cross(front, globalUp));
	localUp = glm::normalize(glm::cross(right, front));
}