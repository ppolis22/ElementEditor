#pragma once

#include "SelectableState.h"
#include "../engine/RayTracer.h"

class SubtractState : public SelectableState {
public:
	SubtractState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	void init() override;
	State getType() override;
};