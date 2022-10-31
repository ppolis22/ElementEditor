#pragma once

#include "MoveableSelectionState.h"
#include "editor/model/Block.h"

#include "engine/app/TranslateHandles.h"
#include "engine/geom/RayTracer.h"
#include "engine/geom/Point3d.h"

#include <unordered_map>
#include <limits>

class MoveState : public MoveableSelectionState {
public:
	MoveState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection);

	State getType() override;

private:
	struct SelectionLimits {
		int minX = std::numeric_limits<int>::max();
		int minY = std::numeric_limits<int>::max();
		int minZ = std::numeric_limits<int>::max();
		int maxX = std::numeric_limits<int>::min();
		int maxY = std::numeric_limits<int>::min();
		int maxZ = std::numeric_limits<int>::min();
	} selectionLimits;

	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection;
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> coveredModelCopy;
	glm::vec3 averageSelectionPoint;

	glm::vec3 getHandlePositionForSelection() override;
	void onMovement() override;
	bool movementIsValid(Point3di attemptedVector) override;

	void moveSelection();
	glm::vec3 averagePoints(const std::unordered_map<Point3di, BlockColor, Point3di::HashFunction>& points);
};