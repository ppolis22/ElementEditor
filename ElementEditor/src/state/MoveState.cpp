#include "MoveState.h"

#include "../engine/ModelRenderer.h"
#include "../AppController.h"
#include "../ChunkManager.h"
#include "../engine/Camera.h"

#include <vector>
#include <algorithm>

MoveState::MoveState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection)
	: MoveableSelectionState(context),
	selection(selection)
{
	for (const auto& entry : selection) {
		coveredModelCopy.emplace(entry.first, BlockColor::EMPTY());
	}
}

State MoveState::getType() {
	return State::MOVE;
}

glm::vec3 MoveState::getHandlePositionForSelection() {
	return averagePoints(selection) + Chunk::HALF_BLOCK_WIDTH;
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