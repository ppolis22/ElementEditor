#pragma once

#include "SelectableState.h"
#include "../engine/RayTracer.h"

class SelectState : public SelectableState {
public:
	SelectState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;

private:
	void addToSelection(Point3di& block);
	void removeFromSelection(Point3di& block);
};