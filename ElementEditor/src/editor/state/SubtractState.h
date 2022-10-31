#pragma once

#include "BaseEditorState.h"

class SubtractState : public BaseEditorState {
public:
	SubtractState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	void init() override;
	State getType() override;
};