#include "ProjectBounds.h"

#include "engine/Point3d.h"

glm::vec3 ProjectBounds::X_MIN_PLANE_NORMAL = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 ProjectBounds::X_MAX_PLANE_NORMAL = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 ProjectBounds::Y_MIN_PLANE_NORMAL = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 ProjectBounds::Y_MAX_PLANE_NORMAL = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 ProjectBounds::Z_MIN_PLANE_NORMAL = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 ProjectBounds::Z_MAX_PLANE_NORMAL = glm::vec3(0.0f, 0.0f, 1.0f);

ProjectBounds::ProjectBounds() {}

ProjectBounds::ProjectBounds(unsigned int xDim, unsigned int yDim, unsigned int zDim)
	: xDim(xDim), yDim(yDim), zDim(zDim), bounded(true)
{
	rebuildPlaneMeshes((float)xDim, (float)yDim, (float)zDim);
}

void ProjectBounds::setProjectBounds(unsigned int xDim, unsigned int yDim, unsigned int zDim) {
	deletePlaneMeshes();
	if (xDim == 0 || yDim == 0 || zDim == 0) {
		this->bounded = false;
	} else {
		this->bounded = true;
		this->xDim = xDim;
		this->yDim = yDim;
		this->zDim = zDim;
		rebuildPlaneMeshes((float)xDim, (float)yDim, (float)zDim);
	}
}

void ProjectBounds::deletePlaneMeshes() {
	xMinPlane.deleteMesh();
	xMaxPlane.deleteMesh();
	yMinPlane.deleteMesh();
	yMaxPlane.deleteMesh();
	zMinPlane.deleteMesh();
	zMaxPlane.deleteMesh();
}

void ProjectBounds::rebuildPlaneMeshes(float xDim, float yDim, float zDim) {
	xMinPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, 0.0f }, Point3df{ 0.0f, yDim, 0.0f }, Point3df{ 0.0f, yDim, zDim }, Point3df{ 0.0f, 0.0f, zDim });

	xMaxPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ xDim, 0.0f, 0.0f }, Point3df{ xDim, yDim, 0.0f }, Point3df{ xDim, yDim, zDim }, Point3df{ xDim, 0.0f, zDim });

	yMinPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, 0.0f }, Point3df{ xDim, 0.0f, 0.0f }, Point3df{ xDim, 0.0f, zDim }, Point3df{ 0.0f, 0.0f, zDim });

	yMaxPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, yDim, 0.0f }, Point3df{ xDim, yDim, 0.0f }, Point3df{ xDim, yDim, zDim }, Point3df{ 0.0f, yDim, zDim });

	zMinPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, 0.0f }, Point3df{ xDim, 0.0f, 0.0f }, Point3df{ xDim, yDim, 0.0f }, Point3df{ 0.0f, yDim, 0.0f });

	zMaxPlane = meshBuilder.buildPlaneLineMesh(
		Point3df{ 0.0f, 0.0f, zDim }, Point3df{ xDim, 0.0f, zDim }, Point3df{ xDim, yDim, zDim }, Point3df{ 0.0f, yDim, zDim });
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

//static glm::mat4 computeShadowMapProjectionMatrix(float yAngle, float xAngle, int projectWidth, int projectDepth, int projectHeight) {
//	// compute the width of the box
//	float projectionWidth = projectWidth * abs(cos(yAngle)) + projectDepth * abs(sin(yAngle));
//	float tempDepth = projectWidth * abs(sin(yAngle)) + projectDepth * abs(cos(yAngle));
//
//	// compute the depth and height of the box
//	float projectionDepth = tempDepth * abs(cos(xAngle)) + projectHeight * abs(sin(xAngle));
//	float projectionHeight = tempDepth * abs(sin(xAngle)) + projectHeight * abs(cos(xAngle));
//
//	glm::mat4 shadowMapProjectionMat = glm::ortho(		// left, right, down, up, near, far
//		-projectionWidth / 2.0f, projectionWidth / 2.0f,
//		-projectionHeight / 2.0f, projectionHeight / 2.0f,
//		0.01f, projectionDepth);
//
//	return shadowMapProjectionMat;
//}

static glm::mat4 computeShadowMapProjectionMatrix(float yAngle, float xAngle) {
	// compute the width of the box
	float projectionWidth = xDim * abs(cos(yAngle)) + zDim * abs(sin(yAngle));
	float tempDepth = xDim * abs(sin(yAngle)) + zDim * abs(cos(yAngle));

	// compute the depth and height of the box
	float projectionDepth = tempDepth * abs(cos(xAngle)) + yDim * abs(sin(xAngle));
	float projectionHeight = tempDepth * abs(sin(xAngle)) + yDim * abs(cos(xAngle));

	glm::mat4 shadowMapProjectionMat = glm::ortho(		// left, right, down, up, near, far
		-projectionWidth / 2.0f, projectionWidth / 2.0f,
		-projectionHeight / 2.0f, projectionHeight / 2.0f,
		0.01f, projectionDepth);

	return shadowMapProjectionMat;
}

static glm::mat4 computeShadowMapPoint(float yAngle, float xAngle) {
	// find x and z coords before y axis rotation
	float b = xDim * abs(cos(yAngle));
	float a = zDim * abs(sin(yAngle));
	float hyp = (b - a) / 2.0f;

	float projectionWidth = xDim * abs(cos(yAngle)) + zDim * abs(sin(yAngle));
	float tempDepth = xDim * abs(sin(yAngle)) + zDim * abs(cos(yAngle));

	// compute the depth and height of the box
	float projectionDepth = tempDepth * abs(cos(xAngle)) + yDim * abs(sin(xAngle));
	float projectionHeight = tempDepth * abs(sin(xAngle)) + yDim * abs(cos(xAngle));

	glm::mat4 shadowMapProjectionMat = glm::ortho(		// left, right, down, up, near, far
		-projectionWidth / 2.0f, projectionWidth / 2.0f,
		-projectionHeight / 2.0f, projectionHeight / 2.0f,
		0.01f, projectionDepth);

	return shadowMapProjectionMat;
}