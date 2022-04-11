#include "ExtrudeState.h"

#include "../AppController.h"
#include "../Chunk.h"
#include "../ChunkManager.h"

#include <algorithm>

ExtrudeState::ExtrudeState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection)
	: MoveableSelectionState(context, selection),
	moveVector{ 0, 0, 0 }
{
	for (const auto& entry : selection) {
		coveredModelCopy.emplace(entry.first, BlockColor{ 0, 0, 0, true });
		extrusion.emplace(entry.first, entry.second);
	}
}

void ExtrudeState::processMouseDown(MouseButtonDownEvent& event) {
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

void ExtrudeState::processMouseMovement(MouseMoveEvent& event) {
	if (moveDirection != NONE) {
		glm::vec3 pointOnAxis = getClosestPointOnAxisToMouse(event.rawX, event.rawY);
		handles.setPosition(pointOnAxis - handleGrabPointOffset);
		float deltaX = std::floor(pointOnAxis.x + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.x);
		float deltaY = std::floor(pointOnAxis.y + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.y);
		float deltaZ = std::floor(pointOnAxis.z + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.z);

		if (std::max({ std::abs(deltaX), std::abs(deltaY), std::abs(deltaZ) }) > 0.0f) {
			moveVector += Point3di{ (int)deltaX, (int)deltaY, (int)deltaZ };
			movementReferencePoint += glm::vec3(deltaX, deltaY, deltaZ);
			setExtrusion();
		}
	}
}

void ExtrudeState::processMouseUp(MouseButtonUpEvent& event) {
	moveDirection = NONE;
	handles.setSelectedDirection(NONE);
	handles.setPosition(averagePoints(selection) + Chunk::HALF_BLOCK_WIDTH + glm::vec3(moveVector.x, moveVector.y, moveVector.z));
}

void ExtrudeState::render() {
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

void ExtrudeState::setExtrusion() {
	// replace blocks extrusion was covering from copy
	ChunkManager* chunkManager = context->getModelChunkManager();
	for (const auto& entry : coveredModelCopy) {
		chunkManager->setBlockColor(entry.second, entry.first);
		chunkManager->setSelected(false, entry.first);
	}

	calculateExtrusion();

	// copy the next blocks under the new extrusion, this should be done after all blocks are replaced
	coveredModelCopy.clear();
	for (auto& entry : extrusion) {
		Point3di extrusionPoint = entry.first;
		coveredModelCopy.emplace(extrusionPoint, chunkManager->getBlockColor(extrusionPoint));
		// set extrusion
		chunkManager->setBlockColor(entry.second, extrusionPoint);
		chunkManager->setSelected(true, extrusionPoint);
	}
	chunkManager->rebuildChunkMeshes();
}

// utilizing Bresenham's algorithm in 3D to draw the extrusion
void ExtrudeState::calculateExtrusion() {
	extrusion.clear();

	int x1 = 0, y1 = 0, z1 = 0;
	int x2 = moveVector.x;
	int y2 = moveVector.y;
	int z2 = moveVector.z;
	int dx = std::abs(x2);
	int dy = std::abs(y2);
	int dz = std::abs(z2);
	int xInc = x2 > x1 ? 1 : -1;
	int yInc = y2 > y1 ? 1 : -1;
	int zInc = z2 > z1 ? 1 : -1;
	addSelectionAtOffset(0, 0, 0);

	if (dx >= dy && dx >= dz) {
		int yError = 2 * dy - dx;
		int zError = 2 * dz - dx;
		while (x1 != x2) {
			x1 += xInc;
			if (yError >= 0) {
				y1 += yInc;
				yError -= 2 * dx;
			}
			if (zError >= 0) {
				z1 += zInc;
				zError -= 2 * dx;
			}
			yError += 2 * dy;
			zError += 2 * dz;
			addSelectionAtOffset(x1, y1, z1);
		}
	} else if (dy >= dx && dy >= dz) {
		int xError = 2 * dx - dy;
		int zError = 2 * dz - dy;
		while (y1 != y2) {
			y1 += yInc;
			if (xError >= 0) {
				x1 += xInc;
				xError -= 2 * dy;
			}
			if (zError >= 0) {
				z1 += zInc;
				zError -= 2 * dy;
			}
			xError += 2 * dx;
			zError += 2 * dz;
			addSelectionAtOffset(x1, y1, z1);
		}
	} else {
		int xError = 2 * dx - dz;
		int yError = 2 * dy - dz;
		while (z1 != z2) {
			z1 += zInc;
			if (xError >= 0) {
				x1 += xInc;
				xError -= 2 * dz;
			}
			if (yError >= 0) {
				y1 += yInc;
				yError -= 2 * dz;
			}
			xError += 2 * dx;
			yError += 2 * dy;
			addSelectionAtOffset(x1, y1, z1);
		}
	}
}

void ExtrudeState::addSelectionAtOffset(int x, int y, int z) {
	for (const auto& entry : selection) {
		Point3di extrusionPoint = Point3di{ x, y, z } + entry.first;
		extrusion.emplace(extrusionPoint, entry.second);
	}
}