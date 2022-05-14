#pragma once

#include "BaseEditorState.h"
#include "../engine/Point3d.h"

class AddState : public BaseEditorState {
public:
	AddState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	void cleanUp() override;
	void init() override;
	void render() override;
	State getType() override;

private:
	Point3di toAddCoords;
	bool readyToAdd = false;
	bool lastBlockWasInBoundsAndEmpty = false;

	bool locationIsEmpty(Point3di location);
};