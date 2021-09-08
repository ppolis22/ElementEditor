#pragma once

#include "BaseEditorState.h"

class SubtractState : public BaseEditorState {
public:
	void processClick(MouseButtonUpEvent& event) override;
};