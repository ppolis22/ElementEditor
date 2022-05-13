#pragma once

#include "MoveableSelectionState.h"
#include "../engine/TranslateHandles.h"
#include "../engine/RayTracer.h"
#include "../engine/Point3d.h"

#include <unordered_map>

class MoveState : public MoveableSelectionState {
public:
	MoveState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection);

	State getType() override;

private:
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection;
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> coveredModelCopy;
	glm::vec3 averageSelectionPoint;

	glm::vec3 getHandlePositionForSelection() override;
	void onMovement() override;

	void moveSelection();
	glm::vec3 averagePoints(const std::unordered_map<Point3di, BlockColor, Point3di::HashFunction>& points);
};