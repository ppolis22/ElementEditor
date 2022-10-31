#pragma once

#include "BaseEditorState.h"

class ColorPickState : public BaseEditorState {
public:
	ColorPickState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	State getType() override;
};