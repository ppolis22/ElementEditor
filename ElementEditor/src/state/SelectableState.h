#pragma once

#include "BaseEditorState.h"
#include "../engine/RayTracer.h"

class SelectableState : public BaseEditorState {
public:
	SelectableState(AppController* context);
	virtual ~SelectableState() {};

protected:
	RayTracer rayTracer;
};