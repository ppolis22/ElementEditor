#pragma once

#include "EditorState.h"

class AddState : public EditorState {
public:
	void processLeftClick(float posX, float posY) override;
	void render() override;
};