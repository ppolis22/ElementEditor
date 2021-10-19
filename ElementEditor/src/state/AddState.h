#pragma once

#include "SelectableState.h"
#include "../engine/RayTracer.h"
#include "../engine/Point3d.h"

class AddState : public SelectableState {
public:
	AddState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	void cleanUp() override;
	void init() override;

private:
	Point3di toAddCoords;
	bool readyToAdd = false;
};