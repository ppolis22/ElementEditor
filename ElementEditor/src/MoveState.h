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
	glm::vec3 handleInitialGrabPoint;
	glm::vec3 handleGrabPointOffset;

	Direction getHandleAtPoint(float x, float y);
	glm::vec3 averagePoints(const std::vector<Point3di>& points);
	glm::vec3 getClosestPointOnAxisToMouse(float mouseX, float mouseY);
	glm::vec3 closestPointOnLineToOtherLine(const glm::vec3& line1Pt1, const glm::vec3& line1Dir,
		const glm::vec3& line2Pt1, const glm::vec3& line2Dir);
	float calculateTerm(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC, const glm::vec3& ptD);
};