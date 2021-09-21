#include "MoveState.h"

#include "engine/ModelRenderer.h"
#include "AppController.h"
#include "ChunkManager.h"
#include "engine/Camera.h"

#include <vector>
#include <algorithm>

MoveState::MoveState(AppController* context, std::unordered_map<Point3di, BlockType, Point3di::HashFunction> selection)
	: BaseEditorState(context),
	selection(selection),
	moveVector{0, 0, 0},
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 10.0f) 
{
	handles.setPosition(averagePoints(selection) + Chunk::HALF_BLOCK_WIDTH);
	for (const auto& entry : selection) {
		coveredModelCopy.emplace(entry.first, Empty);
	}
}

void MoveState::cleanUp() {
	// TODO delete handle mesh
}

void MoveState::processMouseMovement(MouseMoveEvent& event) {
	if (moveDirection != NONE) {
		// get closest point from mouse ray to handle ray
		glm::vec3 pointOnAxis = getClosestPointOnAxisToMouse(event.rawX, event.rawY);
		// update handle position
		handles.setPosition(pointOnAxis - handleGrabPointOffset);
		// update mesh if handle moved full block width
		float handleMoveDistance = glm::length(pointOnAxis - handleInitialGrabPoint);
		if (handleMoveDistance > 1.0f) {
			float deltaX = std::floor(pointOnAxis.x - handleInitialGrabPoint.x);
			float deltaY = std::floor(pointOnAxis.y - handleInitialGrabPoint.y);
			float deltaZ = std::floor(pointOnAxis.z - handleInitialGrabPoint.z);
			moveVector += Point3di{ (int)deltaX, (int)deltaY, (int)deltaZ };
			handleInitialGrabPoint += glm::vec3(deltaX, deltaY, deltaZ);

			// replace blocks selection is covering from copy
			ChunkManager* chunkManager = context->getModelChunkManager();
			for (const auto& entry : coveredModelCopy) {
				chunkManager->setBlock(entry.second, entry.first);
				chunkManager->setSelected(false, entry.first);
			}

			// copy the next blocks at the new offset, this should be done after all blocks are replaced
			coveredModelCopy.clear();
			for (auto& entry : selection) {
				Point3di offsetPoint = moveVector + entry.first;
				coveredModelCopy.emplace(offsetPoint, chunkManager->getBlock(offsetPoint));
				// set selection at offsetPoint
				chunkManager->setBlock(entry.second, offsetPoint);
				chunkManager->setSelected(true, offsetPoint);
			}
			chunkManager->rebuildChunkMeshes();
		}
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
	if (moveDirection != NONE) {
		handles.setSelectedDirection(moveDirection);
		// store closest point from mouse ray to corresponding handle ray
		handleInitialGrabPoint = getClosestPointOnAxisToMouse(event.posX, event.posY);
		handleGrabPointOffset = handleInitialGrabPoint - handles.getPosition();
	}
}

void MoveState::processMouseUp(MouseButtonUpEvent& event) {
	moveDirection = NONE;
	handles.setSelectedDirection(NONE);
	handles.setPosition(averagePoints(selection) + Chunk::HALF_BLOCK_WIDTH + glm::vec3(moveVector.x, moveVector.y, moveVector.z));
}

void MoveState::render() {
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	Camera* camera = context->getCamera();

	for (Chunk& chunk : modelChunkManager->getAllChunks()) {
		modelRenderer->render(chunk, *camera);
	}
	// clear depth buffer to ensure handles are rendered on top of the model
	glClear(GL_DEPTH_BUFFER_BIT);
	modelRenderer->render(handles, *camera);
}

Direction MoveState::getHandleAtPoint(float x, float y) {
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

glm::vec3 MoveState::averagePoints(const std::unordered_map<Point3di, BlockType, Point3di::HashFunction>& points) {
	glm::vec3 average(0.0f, 0.0f, 0.0f);
	for (const auto& entry : points) {
		Point3di point = entry.first;
		average += glm::vec3((float)point.x, (float)point.y, (float)point.z);
	}
	return average / (float)points.size();
}

glm::vec3 MoveState::getClosestPointOnAxisToMouse(float mouseX, float mouseY) {
	Camera* camera = context->getCamera();
	glm::vec3 mouseDirVector = rayTracer.getRayFromScreenCoords(camera->getViewMatrix(), mouseX, mouseY);
	return closestPointOnLineToOtherLine(handles.getPosition(), handles.getAxisVector(moveDirection), camera->getPosition(), mouseDirVector);
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