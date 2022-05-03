#pragma once

#include "MoveableSelectionState.h"
#include "../engine/Light.h"

class MoveLightState : public MoveableSelectionState {
public:
	MoveLightState(AppController* context, Light* lightToMove);

	State getType() override;

private:
	glm::vec3 getHandlePositionForSelection() override;
	void onMovement() override;

	Light* lightToMove;
};