#include "MoveState.h"

#include "../engine/ModelRenderer.h"
#include "../AppController.h"
#include "../ChunkManager.h"
#include "../engine/Camera.h"

#include <vector>
#include <algorithm>

MoveState::MoveState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection)
	: MoveableSelectionState(context),
	selection(selection),
	averageSelectionPoint(averagePoints(selection))
{
	for (const auto& entry : selection) {
		coveredModelCopy.emplace(entry.first, BlockColor::EMPTY());

		selectionLimits.minX = (entry.first.x < selectionLimits.minX) ? entry.first.x : selectionLimits.minX;
		selectionLimits.minY = (entry.first.y < selectionLimits.minY) ? entry.first.y : selectionLimits.minY;
		selectionLimits.minZ = (entry.first.z < selectionLimits.minZ) ? entry.first.z : selectionLimits.minZ;
		selectionLimits.maxX = (entry.first.x > selectionLimits.maxX) ? entry.first.x : selectionLimits.maxX;
		selectionLimits.maxY = (entry.first.y > selectionLimits.maxY) ? entry.first.y : selectionLimits.maxY;
		selectionLimits.maxZ = (entry.first.z > selectionLimits.maxZ) ? entry.first.z : selectionLimits.maxZ;
	}
}

State MoveState::getType() {
	return State::MOVE;
}

glm::vec3 MoveState::getHandlePositionForSelection() {
	return averageSelectionPoint + glm::vec3(moveVector.x, moveVector.y, moveVector.z) + Chunk::HALF_BLOCK_WIDTH;
}

bool MoveState::movementIsValid(Point3di attemptedVector) {
	ProjectBounds* projectBounds = context->getProjectBounds();
	if (!projectBounds->isBounded())
		return true;

	return selectionLimits.minX + attemptedVector.x >= 0 && selectionLimits.maxX + attemptedVector.x < projectBounds->getXDim() &&
		selectionLimits.minY + attemptedVector.y >= 0 && selectionLimits.maxY + attemptedVector.y < projectBounds->getYDim() &&
		selectionLimits.minZ + attemptedVector.z >= 0 && selectionLimits.maxZ + attemptedVector.z < projectBounds->getZDim();
}

void MoveState::onMovement() {
	moveSelection();
}

void MoveState::moveSelection() {
	// replace blocks selection was covering from copy
	ChunkManager* chunkManager = context->getModelChunkManager();
	for (const auto& entry : coveredModelCopy) {
		chunkManager->setBlockColor(entry.second, entry.first);
		chunkManager->setSelected(false, entry.first);
	}

	// copy the next blocks at the new offset, this should be done after all blocks are replaced
	coveredModelCopy.clear();
	for (auto& entry : selection) {
		Point3di offsetPoint = moveVector + entry.first;
		coveredModelCopy.emplace(offsetPoint, chunkManager->getBlockColor(offsetPoint));
		// set selection at offsetPoint
		chunkManager->setBlockColor(entry.second, offsetPoint);
		chunkManager->setSelected(true, offsetPoint);
	}
	chunkManager->rebuildChunkMeshes();
}

glm::vec3 MoveState::averagePoints(const std::unordered_map<Point3di, BlockColor, Point3di::HashFunction>& points) {
	glm::vec3 average(0.0f, 0.0f, 0.0f);
	for (const auto& entry : points) {
		Point3di point = entry.first;
		average += glm::vec3((float)point.x, (float)point.y, (float)point.z);
	}
	return average / (float)points.size();
}