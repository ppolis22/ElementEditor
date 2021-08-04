#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

class Camera {
public:
	Camera();
	~Camera();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

private:
	const float FOV = 45.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
};