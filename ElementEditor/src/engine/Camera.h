#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

class Camera {
public:
	Camera();
	~Camera();

	void rotate(float deltaX, float deltaY);
	void pan(float deltaX, float deltaY);
	void zoom(float deltaY);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::vec3 getPosition();
	glm::vec3 getViewVector();
	glm::vec3 getTarget();

	void setPosition(glm::vec3 position);
	void setTarget(glm::vec3 target);

private:
	const float FOV = 45.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;

	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 globalUp;
	glm::vec3 localUp;
	glm::vec3 right;
	glm::vec3 front;

	float distance;	// distance
	float yaw;		// rotation around (in this case, global) y axis
	float pitch;	// rotation around local x axis
	float panSpeed;
	float rotateSpeed;
	float zoomSpeed;

	glm::mat4 projectionMatrix;

	void updateVectors(glm::vec3 position, glm::vec3 target);
};