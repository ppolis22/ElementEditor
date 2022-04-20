#pragma once

#include "SelectableState.h"
#include "../engine/TranslateHandles.h"
#include "../engine/RayTracer.h"
#include "../engine/Point3d.h"

#include <unordered_map>

class MoveableSelectionState : public SelectableState {
public:
	~MoveableSelectionState();

	void render() override;

protected:
	MoveableSelectionState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection);

	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection;
	Direction moveDirection = NONE;
	TranslateHandles handles;

	Direction getHandleAtPoint(float x, float y);
	glm::vec3 averagePoints(const std::unordered_map<Point3di, BlockColor, Point3di::HashFunction>& points);
	glm::vec3 getClosestPointOnAxisToMouse(float mouseX, float mouseY);
	glm::vec3 closestPointOnLineToOtherLine(const glm::vec3& line1Pt1, const glm::vec3& line1Dir,
		const glm::vec3& line2Pt1, const glm::vec3& line2Dir);
	float calculateTerm(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC, const glm::vec3& ptD);
};