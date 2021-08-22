#include "RayTracer.h"
#include "Point3d.h"

#include <iostream>
#include <algorithm>
#include <vector>

RayTracer::RayTracer(int windowWidth, int windowHeight, glm::mat4 projectionMatrix, float searchLength)
	: windowWidth(windowWidth), windowHeight(windowHeight), inverseProjectionMatrix(glm::inverse(projectionMatrix)), searchLength(searchLength) {}

RayTracer::~RayTracer() {}

std::vector<Point3di> RayTracer::traceRay(const glm::vec3& startPos, const glm::mat4& viewMatrix, float mousePosX, float mousePosY) {
	glm::vec3 ray = getRayFromScreenCoords(viewMatrix, mousePosX, mousePosY);
	return getIntersectingBlocks(startPos, ray);
}

glm::vec3 RayTracer::getRayFromScreenCoords(const glm::mat4& viewMatrix, float mousePosX, float mousePosY) {
	std::cout << "Pixels: (" << mousePosX << ", " << mousePosY << ")" << std::endl;

	float normalizedX = (2.0f * mousePosX) / windowWidth - 1.0f;
	float normalizedY = 1.0f - (2.0f * mousePosY) / windowHeight;	// mouse Y comes in inverted, this corrects for that

	std::cout << "Norm coords: (" << normalizedX << ", " << normalizedY << ")" << std::endl;

	// w must be 1 if x and y are perspective-divided coords, I think z can be anything? Test this
	glm::vec4 clipSpaceCoords(normalizedX, normalizedY, -1.0, 1.0);

	std::cout << "Clip space: (" << clipSpaceCoords.x << ", " << clipSpaceCoords.y << ", " << clipSpaceCoords.z << ", " << clipSpaceCoords.w << ")" << std::endl;

	glm::vec4 eyeSpaceCoords = inverseProjectionMatrix * clipSpaceCoords;
	// here z is hardcoded to -1, but I think that only works if clip space w coord is also hardcoded to 1
	// w set to 0 so this represents a vector rather than a point
	eyeSpaceCoords = glm::vec4(eyeSpaceCoords.x, eyeSpaceCoords.y, -1.0, 0.0);

	std::cout << "Eye space: (" << eyeSpaceCoords.x << ", " << eyeSpaceCoords.y << ", " << eyeSpaceCoords.z << ", " << eyeSpaceCoords.w << ")" << std::endl;

	glm::vec4 worldRay = glm::inverse(viewMatrix) * eyeSpaceCoords;
	glm::vec3 normalizedWorldRay = glm::normalize(glm::vec3(worldRay.x, worldRay.y, worldRay.z));

	std::cout << "World ray: (" << normalizedWorldRay.x << ", " << normalizedWorldRay.y << ", " << normalizedWorldRay.z << ")" << std::endl;

	return normalizedWorldRay;
}

std::vector<Point3di> RayTracer::getIntersectingBlocks(const glm::vec3& startPos, const glm::vec3& direction) {
	glm::vec3 endPos = (direction * searchLength) + startPos;
	float dx = fabs(endPos.x - startPos.x);
	float dy = fabs(endPos.y - startPos.y);
	float dz = fabs(endPos.z - startPos.z);

	int x = int(floor(startPos.x));
	int y = int(floor(startPos.y));
	int z = int(floor(startPos.z));

	float dt_dx = 1.0f / dx;
	float dt_dy = 1.0f / dy;
	float dt_dz = 1.0f / dz;

	int n = 1;
	int x_inc, y_inc, z_inc;
	float t_next_vertical, t_next_horizontal, t_next_inward;
	std::vector<Point3di> visited;

	if (dx == 0) {
		x_inc = 0;
		t_next_horizontal = dt_dx; // infinity
	} else if (endPos.x > startPos.x) {
		x_inc = 1;
		n += int(floor(endPos.x)) - x;
		t_next_horizontal = (floor(startPos.x) + 1 - startPos.x) * dt_dx;
	} else {
		x_inc = -1;
		n += x - int(floor(endPos.x));
		t_next_horizontal = (startPos.x - floor(startPos.x)) * dt_dx;
	}

	if (dy == 0) {
		y_inc = 0;
		t_next_vertical = dt_dy; // infinity
	} else if (endPos.y > startPos.y) {
		y_inc = 1;
		n += int(floor(endPos.y)) - y;
		t_next_vertical = (floor(startPos.y) + 1 - startPos.y) * dt_dy;
	} else {
		y_inc = -1;
		n += y - int(floor(endPos.y));
		t_next_vertical = (startPos.y - floor(startPos.y)) * dt_dy;
	}

	if (dz == 0) {
		z_inc = 0;
		t_next_inward = dt_dz; // infinity
	} else if (endPos.z > startPos.z) {
		z_inc = 1;
		n += int(floor(endPos.z)) - z;
		t_next_inward = (floor(startPos.z) + 1 - startPos.z) * dt_dz;
	} else {
		z_inc = -1;
		n += z - int(floor(endPos.z));
		t_next_inward = (startPos.z - floor(startPos.z)) * dt_dz;
	}

	for (; n > 0; --n) {
		visited.push_back({ x, y, z });

		float min_next(std::min({t_next_horizontal, t_next_vertical, t_next_inward}));
		if (min_next == t_next_horizontal) {
			x += x_inc;
			t_next_horizontal += dt_dx;
		} else if (min_next == t_next_vertical) {
			y += y_inc;
			t_next_vertical += dt_dy;
		} else {
			z += z_inc;
			t_next_inward += dt_dz;
		}
	}

	return visited;
}