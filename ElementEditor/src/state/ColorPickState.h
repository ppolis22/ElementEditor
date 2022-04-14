#pragma once

#include "SelectableState.h"

class ColorPickState : public SelectableState {
public:
	ColorPickState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
};