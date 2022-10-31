#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() {
	projectionMatrix = glm::perspective(glm::radians(FOV), 960.0f / 540.0f, NEAR_PLANE, FAR_PLANE);

	position = glm::vec3(0.0f, 0.0f, -5.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	localUp = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	distance = 5.0f;
	yaw = 0.0f;
	pitch = 0.0f;
	panSpeed = 0.0008f;
	rotateSpeed = 0.25f;
	zoomSpeed = 0.75f;
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

glm::vec3 Camera::getViewVector() {
	return glm::normalize(target - position);
}

glm::vec3 Camera::getTarget() {
	return target;
}

void Camera::updateVectors(glm::vec3 position, glm::vec3 target) {
	distance = glm::length(position - target);
	front = glm::normalize(position - target);
	right = glm::normalize(glm::cross(front, globalUp));
	localUp = glm::normalize(glm::cross(right, front));

	yaw = glm::degrees(atan2(front.x, front.z));
	pitch = glm::degrees(asin(front.y));
}

void Camera::setPosition(glm::vec3 position) {
	this->position = position;
	updateVectors(position, target);
}

void Camera::setTarget(glm::vec3 target) {
	this->target = target;
	updateVectors(position, target);
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
	target += (right * deltaX * panSpeed * (distance + 5.0f));
	target += (localUp * deltaY * panSpeed * (distance + 5.0f));

	position += (right * deltaX * panSpeed * (distance + 5.0f));
	position += (localUp * deltaY * panSpeed * (distance + 5.0f));
}

void Camera::zoom(float deltaY) {
	float distanceChange = deltaY * zoomSpeed;
	distance = (distanceChange <= -distance ? 0.0001f : distance + distanceChange);
	position = (front * distance) + target;
}