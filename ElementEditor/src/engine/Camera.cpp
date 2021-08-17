#include "Camera.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

Camera::Camera() {
	projectionMatrix = glm::perspective(glm::radians(FOV), 960.0f / 540.0f, NEAR_PLANE, FAR_PLANE);

	position = glm::vec3(0.0f, 0.0f, 5.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	localUp = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	distance = 5.0f;
	yaw = -90.f;
	pitch = 0.0f;
	panSpeed = 0.01f;
	rotateSpeed = 0.25f;
	zoomSpeed = 0.5f;
}

Camera::~Camera() {}

glm::mat4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, target, globalUp);
}

glm::vec3 Camera::getPosition() {
	return position;
}

void Camera::rotate(float deltaX, float deltaY) {
	yaw += (deltaX * rotateSpeed);
	pitch += (deltaY * rotateSpeed);

	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	position = (front * distance) + target;
	right = glm::normalize(glm::cross(front, globalUp));
	localUp = glm::normalize(glm::cross(right, front));
}

void Camera::pan(float deltaX, float deltaY) {
	target += (right * deltaX * panSpeed);
	target += (localUp * deltaY * panSpeed);

	position += (right * deltaX * panSpeed);
	position += (localUp * deltaY * panSpeed);
}

void Camera::zoom(float deltaY) {
	distance += deltaY * zoomSpeed * sqrt(distance / 10.0f);	// TODO fix this
	position = (front * distance) + target;
}