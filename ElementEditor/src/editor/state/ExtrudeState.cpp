#include "ExtrudeState.h"

#include "editor/controller/AppController.h"
#include "editor/controller/ChunkManager.h"
#include "editor/model/Chunk.h"

#include <algorithm>

ExtrudeState::ExtrudeState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection)
	: MoveableSelectionState(context),
	selection(selection),
	averageSelectionPoint(averagePoints(selection))
{
	for (const auto& entry : selection) {
		coveredModelCopy.emplace(entry.first, BlockColor::EMPTY());
		extrusion.emplace(entry.first, entry.second);

		selectionLimits.minX = (entry.first.x < selectionLimits.minX) ? entry.first.x : selectionLimits.minX;
		selectionLimits.minY = (entry.first.y < selectionLimits.minY) ? entry.first.y : selectionLimits.minY;
		selectionLimits.minZ = (entry.first.z < selectionLimits.minZ) ? entry.first.z : selectionLimits.minZ;
		selectionLimits.maxX = (entry.first.x > selectionLimits.maxX) ? entry.first.x : selectionLimits.maxX;
		selectionLimits.maxY = (entry.first.y > selectionLimits.maxY) ? entry.first.y : selectionLimits.maxY;
		selectionLimits.maxZ = (entry.first.z > selectionLimits.maxZ) ? entry.first.z : selectionLimits.maxZ;
	}
}

State ExtrudeState::getType() {
	return State::EXTRUDE;
}

glm::vec3 ExtrudeState::getHandlePositionForSelection() {
	return averageSelectionPoint + glm::vec3(moveVector.x, moveVector.y, moveVector.z) + Chunk::HALF_BLOCK_WIDTH;
}

bool ExtrudeState::movementIsValid(Point3di attemptedVector) {
	ProjectBounds* projectBounds = context->getProjectBounds();
	if (!projectBounds->isBounded())
		return true;

	return selectionLimits.minX + attemptedVector.x >= 0 && selectionLimits.maxX + attemptedVector.x < projectBounds->getXDim() &&
		selectionLimits.minY + attemptedVector.y >= 0 && selectionLimits.maxY + attemptedVector.y < projectBounds->getYDim() &&
		selectionLimits.minZ + attemptedVector.z >= 0 && selectionLimits.maxZ + attemptedVector.z < projectBounds->getZDim();
}

void ExtrudeState::onMovement() {
	setExtrusion();
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
	context->rebuildShadowMap();
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

glm::vec3 ExtrudeState::averagePoints(const std::unordered_map<Point3di, BlockColor, Point3di::HashFunction>& points) {
	glm::vec3 average(0.0f, 0.0f, 0.0f);
	for (const auto& entry : points) {
		Point3di point = entry.first;
		average += glm::vec3((float)point.x, (float)point.y, (float)point.z);
	}
	return average / (float)points.size();
}