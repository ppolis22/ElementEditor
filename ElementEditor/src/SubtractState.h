#pragma once

#include "BaseEditorState.h"
#include "engine/RayTracer.h"

class SubtractState : public BaseEditorState {
public:
	SubtractState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;

private:
	RayTracer rayTracer;
};