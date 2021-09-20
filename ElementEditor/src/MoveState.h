#pragma once

#include "BaseEditorState.h"
#include "engine/TranslateHandles.h"
#include "engine/RayTracer.h"
#include "engine/Point3d.h"

#include <vector>

class MoveState : public BaseEditorState {
public:
	MoveState(AppController* context, std::vector<Point3di> selection);

	void processMouseDown(MouseButtonDownEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	void render() override;

private:
	std::vector<Point3di> selection;
	Direction moveDirection = NONE;
	TranslateHandles handles;
	RayTracer rayTracer;

	Direction getHandleAtPoint(float x, float y);
	glm::vec3 averagePoints(const std::vector<Point3di>& points);
};