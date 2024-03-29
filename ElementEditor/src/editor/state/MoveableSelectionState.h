#pragma once

#include "BaseEditorState.h"
#include "engine/app/TranslateHandles.h"
#include "engine/geom/RayTracer.h"
#include "engine/geom/Point3d.h"

#include <unordered_map>

class MoveableSelectionState : public BaseEditorState {
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

	// this method should check that moving along attemptedVector is valid 
	// (e.g. selection is in bounds), otherwise onMovement will not be called
	virtual bool movementIsValid(Point3di attemptedVector) { return true; };

	Direction moveDirection = NONE;
	Point3di moveVector;
	RayTracer rayTracer;

private:
	Direction getHandleAtPoint(float x, float y);
	glm::vec3 getClosestPointOnAxisToMouse(float mouseX, float mouseY);

	TranslateHandles handles;
	glm::vec3 movementReferencePoint;
	glm::vec3 handleGrabPointOffset;
	bool isMoving;

};