#pragma once

#include "SelectableState.h"
#include "../engine/TranslateHandles.h"
#include "../engine/RayTracer.h"
#include "../engine/Point3d.h"

#include <unordered_map>

//template <class T>
//using Selection = std::unordered_map<Point3di, T, Point3di::HashFunction>;

class MoveableSelectionState : public SelectableState {
public:
	virtual ~MoveableSelectionState() {};

	void processMouseDown(MouseButtonDownEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	void render() override;
	void init() override;

protected:
	MoveableSelectionState(AppController* context);

	// this method should return the coordinates of the movement handles for
	// any given selection
	virtual glm::vec3 getHandlePositionForSelection() = 0;

	// this method should provide desired behavior in response to moveVector changing
	virtual void onMovement() = 0;

	Direction moveDirection = NONE;
	Point3di moveVector;

private:
	Direction getHandleAtPoint(float x, float y);
	glm::vec3 getClosestPointOnAxisToMouse(float mouseX, float mouseY);

	TranslateHandles handles;
	glm::vec3 movementReferencePoint;
	glm::vec3 handleGrabPointOffset;

};