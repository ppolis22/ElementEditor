#include "MoveState.h"

#include "../engine/ModelRenderer.h"
#include "../AppController.h"
#include "../ChunkManager.h"
#include "../engine/Camera.h"

#include <vector>
#include <algorithm>

MoveState::MoveState(AppController* context, std::unordered_map<Point3di, BlockType, Point3di::HashFunction> selection)
	: MoveableSelectionState(context, selection),
	moveVector{0, 0, 0}
{
	for (const auto& entry : selection) {
		coveredModelCopy.emplace(entry.first, Empty);
	}
}

void MoveState::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	moveDirection = getHandleAtPoint(event.posX, event.posY);
	if (moveDirection != NONE) {
		handles.setSelectedDirection(moveDirection);
		// store closest point from mouse ray to corresponding handle ray
		movementReferencePoint = getClosestPointOnAxisToMouse(event.posX, event.posY);
		handleGrabPointOffset = movementReferencePoint - handles.getPosition();
	}
}

void MoveState::processMouseMovement(MouseMoveEvent& event) {
	if (moveDirection != NONE) {
		glm::vec3 pointOnAxis = getClosestPointOnAxisToMouse(event.rawX, event.rawY);
		handles.setPosition(pointOnAxis - handleGrabPointOffset);
		float deltaX = std::floor(pointOnAxis.x + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.x);
		float deltaY = std::floor(pointOnAxis.y + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.y);
		float deltaZ = std::floor(pointOnAxis.z + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.z);

		if (std::max({ std::abs(deltaX), std::abs(deltaY), std::abs(deltaZ) }) > 0.0f) {
			moveVector += Point3di{ (int)deltaX, (int)deltaY, (int)deltaZ };
			movementReferencePoint += glm::vec3(deltaX, deltaY, deltaZ);
			moveSelection();
		}
	} 
	//else {
	//	Direction hoveredDirection = getHandleAtPoint(event.rawX, event.rawY);
	//	// highlight axis handle for hoveredDirection?
	//}
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

void MoveState::moveSelection() {
	// replace blocks selection was covering from copy
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