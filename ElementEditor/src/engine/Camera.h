#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include "InputListener.h"

class Camera {
public:
	Camera();
	~Camera();

	void rotate(float deltaX, float deltaY);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

private:
	const float FOV = 45.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;

	glm::vec3 position;
	glm::vec3 globalUp;
	glm::vec3 localUp;
	glm::vec3 front;
	glm::vec3 right;

	float yaw;		// rotation around (in this case, global) y axis
	float pitch;	// rotation around local x axis
	float movementSpeed;
	float mouseSensitivity;

	glm::mat4 projectionMatrix;
};