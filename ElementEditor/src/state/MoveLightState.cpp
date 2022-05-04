#include "MoveLightState.h"

#include "../AppController.h"

MoveLightState::MoveLightState(AppController* context, Light* lightToMove)
	: MoveableSelectionState(context),
	lightToMove(lightToMove),
	initialLightPosition(lightToMove->getBlockPosition())
{}

State MoveLightState::getType() {
	return State::MOVE_LIGHT;
}

glm::vec3 MoveLightState::getHandlePositionForSelection() {
	return lightToMove->getRenderPosition();
}

void MoveLightState::onMovement() {
	lightToMove->setBlockPosition(initialLightPosition + moveVector);
}