#pragma once

#include "BaseEditorState.h"

class RemoveLightState : public BaseEditorState {
public:
	RemoveLightState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	void init() override;
	State getType() override;
};