#pragma once

#include "MoveableSelectionState.h"

#include "../engine/Point3d.h"
#include "../Block.h"

#include <unordered_map>

class ExtrudeState : public MoveableSelectionState {

public:
	ExtrudeState(AppController* context, std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> selection);

	void processMouseDown(MouseButtonDownEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	void render() override;

private:
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> coveredModelCopy;
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> extrusion;
	Point3di moveVector;
	glm::vec3 movementReferencePoint;
	glm::vec3 handleGrabPointOffset;

	void setExtrusion();
	void calculateExtrusion();
	void addSelectionAtOffset(int x, int y, int z);
};