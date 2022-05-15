#include "ProjectBounds.h"

#include "engine/Point3d.h"

glm::vec3 ProjectBounds::X_MIN_PLANE_NORMAL = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 ProjectBounds::X_MAX_PLANE_NORMAL = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 ProjectBounds::Y_MIN_PLANE_NORMAL = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 ProjectBounds::Y_MAX_PLANE_NORMAL = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 ProjectBounds::Z_MIN_PLANE_NORMAL = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 ProjectBounds::Z_MAX_PLANE_NORMAL = glm::vec3(0.0f, 0.0f, 1.0f);

ProjectBounds::ProjectBounds() {}

ProjectBounds::ProjectBounds(int xDim, int yDim, int zDim)
	: xDim(xDim), yDim(yDim), zDim(zDim), bounded(true)
{
	float xDimF = (float)xDim;
	float yDimF = (float)yDim;
	float zDimF = (float)zDim;

	xMinPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, 0.0f }, Point3df{ 0.0f, yDimF, 0.0f }, Point3df{ 0.0f, yDimF, zDimF }, Point3df{ 0.0f, 0.0f, zDimF });

	xMaxPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ xDimF, 0.0f, 0.0f }, Point3df{ xDimF, yDimF, 0.0f }, Point3df{ xDimF, yDimF, zDimF }, Point3df{ xDimF, 0.0f, zDimF });

	yMinPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, 0.0f }, Point3df{ xDimF, 0.0f, 0.0f }, Point3df{ xDimF, 0.0f, zDimF }, Point3df{ 0.0f, 0.0f, zDimF });

	yMaxPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, yDimF, 0.0f }, Point3df{ xDimF, yDimF, 0.0f }, Point3df{ xDimF, yDimF, zDimF }, Point3df{ 0.0f, yDimF, zDimF });

	zMinPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, 0.0f }, Point3df{ xDimF, 0.0f, 0.0f }, Point3df{ xDimF, yDimF, 0.0f }, Point3df{ 0.0f, yDimF, 0.0f });

	zMaxPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, zDimF }, Point3df{ xDimF, 0.0f, zDimF }, Point3df{ xDimF, yDimF, zDimF }, Point3df{ 0.0f, yDimF, zDimF });
}

int ProjectBounds::getXDim() {
	return xDim;
}

int ProjectBounds:: getYDim() {
	return yDim;
}

int ProjectBounds::getZDim() {
	return zDim;
}

bool ProjectBounds::isBounded() {
	return bounded;
}

static bool planeVisible(glm::vec3 cameraDir, glm::vec3 planeNormal) {
	return glm::dot(cameraDir, planeNormal) > 0.0f;
}

std::vector<Mesh*> ProjectBounds::getVisiblePlanes(glm::vec3 cameraDir) {
	std::vector<Mesh*> visiblePlanes;

	if (planeVisible(cameraDir, X_MIN_PLANE_NORMAL))
		visiblePlanes.push_back(&xMinPlane);

	if (planeVisible(cameraDir, X_MAX_PLANE_NORMAL))
		visiblePlanes.push_back(&xMaxPlane);

	if (planeVisible(cameraDir, Y_MIN_PLANE_NORMAL))
		visiblePlanes.push_back(&yMinPlane);

	if (planeVisible(cameraDir, Y_MAX_PLANE_NORMAL))
		visiblePlanes.push_back(&yMaxPlane);

	if (planeVisible(cameraDir, Z_MIN_PLANE_NORMAL))
		visiblePlanes.push_back(&zMinPlane);

	if (planeVisible(cameraDir, Z_MAX_PLANE_NORMAL))
		visiblePlanes.push_back(&zMaxPlane);

	return visiblePlanes;
}