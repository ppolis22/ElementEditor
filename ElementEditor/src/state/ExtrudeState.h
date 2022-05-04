#pragma once

#include "MoveableSelectionState.h"

#include "../engine/Point3d.h"
#include "../Block.h"

#include <unordered_map>

class ExtrudeState : public MoveableSelectionState {

public:
	ExtrudeState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection);

	State getType() override;

private:
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection;
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> coveredModelCopy;
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> extrusion;
	glm::vec3 averageSelectionPoint;

	glm::vec3 getHandlePositionForSelection() override;
	void onMovement() override;

	void setExtrusion();
	void calculateExtrusion();
	void addSelectionAtOffset(int x, int y, int z);

	glm::vec3 averagePoints(const std::unordered_map<Point3di, BlockColor, Point3di::HashFunction>& points);
};