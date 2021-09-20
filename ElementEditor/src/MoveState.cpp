#include "MoveState.h"

#include "engine/ModelRenderer.h"
#include "AppController.h"
#include "ChunkManager.h"
#include "engine/Camera.h"

#include <vector>
#include <iostream>

MoveState::MoveState(AppController* context, std::vector<Point3di> selection)
	: BaseEditorState(context),
	selection(selection),
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 10.0f) 
{
	handles.setPosition(averagePoints(selection) + Chunk::HALF_BLOCK_WIDTH);
}

void MoveState::processMouseMovement(MouseMoveEvent& event) {
	if (moveDirection != NONE) {
		// get closest point from mouse ray to handle ray
		//closestPointOnLineToOtherLine(handles.setPosition(), , glm::vec3(2.0f, 5.0f, -3.0f), glm::vec3(1.0f, 1.0f, 2.0f));

		// calculate change from last movement
		// update handle position
		// update mesh if handle moved full block width
		// update last closest point
	} 
	//else {
	//	Direction hoveredDirection = getHandleAtPoint(event.rawX, event.rawY);
	//	// highlight axis handle for hoveredDirection?
	//}
}

void MoveState::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	moveDirection = getHandleAtPoint(event.posX, event.posY);
	handles.setSelectedDirection(moveDirection);
	// store closest point from mouse ray to corresponding handle ray
}

void MoveState::processMouseUp(MouseButtonUpEvent& event) {
	moveDirection = NONE;
	// commit mesh changes if any were made?
	// unhighlight axis handle?
	handles.setSelectedDirection(NONE);
}

void MoveState::render() {
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	Camera* camera = context->getCamera();

	for (Chunk& chunk : modelChunkManager->getAllChunks()) {
		modelRenderer->render(chunk, *camera);
	}
	// clear depth buffer to endure handles are rendered on top of the model
	glClear(GL_DEPTH_BUFFER_BIT);
	modelRenderer->render(handles, *camera);
}

Direction MoveState::getHandleAtPoint(float x, float y) {
	Camera* camera = context->getCamera();
	glm::vec3 mouseDirVector = rayTracer.getRayFromScreenCoords(camera->getViewMatrix(), x, y);
	Direction targetDirection = NONE;
	float minDistToHandle = RayTracer::MAX_DISTANCE;

	AABB xHandleBox = handles.getXBoundingBox();
	float distToXHandle = rayTracer.getDistanceToTarget(xHandleBox, camera->getPosition(), mouseDirVector);
	if (distToXHandle < minDistToHandle) {
		targetDirection = X;
		minDistToHandle = distToXHandle;
	}

	AABB yHandleBox = handles.getYBoundingBox();
	distToXHandle = rayTracer.getDistanceToTarget(yHandleBox, camera->getPosition(), mouseDirVector);
	if (distToXHandle < minDistToHandle) {
		targetDirection = Y;
		minDistToHandle = distToXHandle;
	}

	AABB zHandleBox = handles.getZBoundingBox();
	distToXHandle = rayTracer.getDistanceToTarget(zHandleBox, camera->getPosition(), mouseDirVector);
	if (distToXHandle < minDistToHandle) {
		targetDirection = Z;
		minDistToHandle = distToXHandle;
	}

	std::cout << "Selected move direction: " << targetDirection << std::endl;
	return targetDirection;
}

glm::vec3 MoveState::averagePoints(const std::vector<Point3di>& points) {
	glm::vec3 average(0.0f, 0.0f, 0.0f);
	for (const Point3di& point : points) {
		average += glm::vec3((float)point.x, (float)point.y, (float)point.z);
	}
	return average / (float)points.size();
}

// based on http://paulbourke.net/geometry/pointlineplane/, the several computed terms serve as a simple solution to 
// a system of equations to solve for the parametric eq variable t to get the target distance along line 1
glm::vec3 MoveState::closestPointOnLineToOtherLine(const glm::vec3& line1Pt1, const glm::vec3& line1Dir,
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

float MoveState::calculateTerm(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC, const glm::vec3& ptD) {
	return (ptA.x - ptB.x) * (ptC.x - ptD.x) + (ptA.y - ptB.y) * (ptC.y - ptD.y) + (ptA.z - ptB.z) * (ptC.z - ptD.z);
}