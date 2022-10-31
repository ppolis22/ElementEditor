#pragma once

#include "Point3d.h"
#include "AABB.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <limits>

class RayTracer {
public:
	RayTracer(int windowWidth, int windowHeight, glm::mat4 projectionMatrix, float searchLength);
	~RayTracer();

	static constexpr float MAX_DISTANCE = std::numeric_limits<float>::infinity();

	std::vector<Point3di> getIntersectingBlocks(const glm::vec3& startPos, const glm::mat4& viewMatrix, float mousePosX, float mousePosY);
	glm::vec3 getRayFromScreenCoords(const glm::mat4& viewMatrix, float mousePosX, float mousePosY);
	float getDistanceToTarget(const AABB& target, const glm::vec3& startPos, const glm::vec3& direction);

private:
	int windowWidth;
	int windowHeight;
	glm::mat4 inverseProjectionMatrix;
	float searchLength;

	std::vector<Point3di> getIntersectingBlocks(const glm::vec3& startPos, const glm::vec3& direction);
};