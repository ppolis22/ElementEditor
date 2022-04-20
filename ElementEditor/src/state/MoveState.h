#pragma once

#include "MoveableSelectionState.h"
#include "../engine/TranslateHandles.h"
#include "../engine/RayTracer.h"
#include "../engine/Point3d.h"

#include <unordered_map>

class MoveState : public MoveableSelectionState {
public:
	MoveState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection);

	void processMouseDown(MouseButtonDownEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	State getType() override;

private:
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> coveredModelCopy;
	Point3di moveVector;
	glm::vec3 movementReferencePoint;
	glm::vec3 handleGrabPointOffset;

	void moveSelection();
};