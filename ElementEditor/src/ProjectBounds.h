#pragma once

#include "engine/LineMeshBuilder.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include <vector>

class ProjectBounds {
public:
	ProjectBounds();
	ProjectBounds(unsigned int xDim, unsigned int yDim, unsigned int zDim);

	int getXDim();
	int getYDim();
	int getZDim();
	bool isBounded();

	void setProjectBounds(unsigned int xDim, unsigned int yDim, unsigned int zDim);
	std::vector<Mesh*> getVisiblePlanes(glm::vec3 cameraDir);

private:
	void deletePlaneMeshes();
	void rebuildPlaneMeshes(float xDim, float yDim, float zDim);

	bool bounded = false;
	unsigned int xDim = 0, yDim = 0, zDim = 0;
	LineMeshBuilder meshBuilder;
	Mesh xMinPlane, xMaxPlane, yMinPlane, yMaxPlane, zMinPlane, zMaxPlane;

	static glm::vec3 X_MIN_PLANE_NORMAL;
	static glm::vec3 X_MAX_PLANE_NORMAL;
	static glm::vec3 Y_MIN_PLANE_NORMAL;
	static glm::vec3 Y_MAX_PLANE_NORMAL;
	static glm::vec3 Z_MIN_PLANE_NORMAL;
	static glm::vec3 Z_MAX_PLANE_NORMAL;
};