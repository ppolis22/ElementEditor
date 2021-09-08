#pragma once

#include "BaseEditorState.h"

class AddState : public BaseEditorState {
public:
	void processClick(MouseButtonUpEvent& event) override;
};