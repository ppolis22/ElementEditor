#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "Point3d.h"

#include <vector>

class RayTracer {
public:
	RayTracer(int windowWidth, int windowHeight, glm::mat4 projectionMatrix, float searchLength);
	~RayTracer();

	std::vector<Point3di> traceRay(const glm::vec3& startPos, const glm::mat4& viewMatrix, float mousePosX, float mousePosY);

private:
	int windowWidth;
	int windowHeight;
	glm::mat4 inverseProjectionMatrix;
	float searchLength;

	glm::vec3 getRayFromScreenCoords(const glm::mat4& viewMatrix, float mousePosX, float mousePosY);
	std::vector<Point3di> getIntersectingBlocks(const glm::vec3& startPos, const glm::vec3& direction);
};