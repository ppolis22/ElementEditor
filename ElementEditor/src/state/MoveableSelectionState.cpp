#include "MoveableSelectionState.h"

#include "../AppController.h"
#include "../Chunk.h"
#include "../engine/Camera.h"

#include <algorithm>

MoveableSelectionState::MoveableSelectionState(AppController* context, std::unordered_map<Point3di, BlockType, Point3di::HashFunction> selection)
	: SelectableState(context),
	selection(selection)
{
	handles.setPosition(averagePoints(selection) + Chunk::HALF_BLOCK_WIDTH);
}

MoveableSelectionState::~MoveableSelectionState() {}

Direction MoveableSelectionState::getHandleAtPoint(float x, float y) {
	Camera* camera = context->getCamera();
	glm::vec3 mouseDirVector = rayTracer.getRayFromScreenCoords(camera->getViewMatrix(), x, y);

	float distToXHandle = rayTracer.getDistanceToTarget(handles.getXBoundingBox(), camera->getPosition(), mouseDirVector);
	float distToYHandle = rayTracer.getDistanceToTarget(handles.getYBoundingBox(), camera->getPosition(), mouseDirVector);
	float distToZHandle = rayTracer.getDistanceToTarget(handles.getZBoundingBox(), camera->getPosition(), mouseDirVector);

	float minDistToHandle = std::min({ distToXHandle , distToYHandle, distToZHandle });
	if (minDistToHandle == RayTracer::MAX_DISTANCE) {
		return NONE;
	} else if (minDistToHandle == distToXHandle) {
		return X;
	} else if (minDistToHandle == distToYHandle) {
		return Y;
	} else {
		return Z;
	}
}

glm::vec3 MoveableSelectionState::averagePoints(const std::unordered_map<Point3di, BlockType, Point3di::HashFunction>& points) {
	glm::vec3 average(0.0f, 0.0f, 0.0f);
	for (const auto& entry : points) {
		Point3di point = entry.first;
		average += glm::vec3((float)point.x, (float)point.y, (float)point.z);
	}
	return average / (float)points.size();
}

glm::vec3 MoveableSelectionState::getClosestPointOnAxisToMouse(float mouseX, float mouseY) {
	Camera* camera = context->getCamera();
	glm::vec3 mouseDirVector = rayTracer.getRayFromScreenCoords(camera->getViewMatrix(), mouseX, mouseY);
	return closestPointOnLineToOtherLine(handles.getPosition(), handles.getAxisVector(moveDirection), camera->getPosition(), mouseDirVector);
}

// based on http://paulbourke.net/geometry/pointlineplane/, the several computed terms serve as a simple solution to 
// a system of equations to solve for the parametric eq variable t to get the target distance along line 1
glm::vec3 MoveableSelectionState::closestPointOnLineToOtherLine(const glm::vec3& line1Pt1, const glm::vec3& line1Dir,
	const glm::vec3& line2Pt1, const glm::vec3& line2Dir) {
	glm::vec3 line1Pt2 = line1Pt1 + line1Dir;
	glm::vec3 line2Pt2 = line2Pt1 + line2Dir;

	float term1343 = calculateTerm(line1Pt1, line2Pt1, line2Pt2, line2Pt1);
	float term4321 = calculateTerm(line2Pt2, line2Pt1, line1Pt2, line1Pt1);
	float term1321 = calculateTerm(line1Pt1, line2Pt1, line1Pt2, line1Pt1);
	float term4343 = calculateTerm(line2Pt2, line2Pt1, line2Pt2, line2Pt1);
	float term2121 = calculateTerm(line1Pt2, line1Pt1, line1Pt2, line1Pt1);

	float t = (term1343 * term4321 - term1321 * term4343) / (term2121 * term4343 - term4321 * term4321);
	return line1Dir * t + line1Pt1;
}

float MoveableSelectionState::calculateTerm(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC, const glm::vec3& ptD) {
	return (ptA.x - ptB.x) * (ptC.x - ptD.x) + (ptA.y - ptB.y) * (ptC.y - ptD.y) + (ptA.z - ptB.z) * (ptC.z - ptD.z);
}