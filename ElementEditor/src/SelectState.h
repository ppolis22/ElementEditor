#pragma once

#include "BaseEditorState.h"
#include "engine/RayTracer.h"

class SelectState : public BaseEditorState {
public:
	SelectState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;

private:
	RayTracer rayTracer;

	void addToSelection(Point3di& block);
	void removeFromSelection(Point3di& block);
};