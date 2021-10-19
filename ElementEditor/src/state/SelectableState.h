#pragma once

#include "BaseEditorState.h"
#include "../engine/RayTracer.h"

class SelectableState : public BaseEditorState {
public:
	SelectableState(AppController* context);

protected:
	RayTracer rayTracer;
};