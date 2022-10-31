#pragma once

#include "BaseEditorState.h"

class AddLightState : public BaseEditorState {
public:
	AddLightState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	void cleanUp() override;
	void init() override;
	State getType() override;

private:
	Point3di toAddCoords;
	bool readyToAdd = false;
};