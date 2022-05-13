#include "LightEditState.h"
#include "../AppController.h"

LightEditState::MoveLightState::MoveLightState(AppController* context, Light* lightToMove)
	: MoveableSelectionState(context),
	lightToMove(lightToMove),
	initialLightPosition(lightToMove->getBlockPosition())
{}

State LightEditState::MoveLightState::getType() {
	return State::EDIT_LIGHT;
}

glm::vec3 LightEditState::MoveLightState::getHandlePositionForSelection() {
	return lightToMove->getRenderPosition();
}

void LightEditState::MoveLightState::onMovement() {
	lightToMove->setBlockPosition(initialLightPosition + moveVector);
}

LightEditState::LightEditState(AppController* context)
	: BaseEditorState(context)
{}

State LightEditState::getType() {
	return EDIT_LIGHT;
}

void LightEditState::processMouseDown(MouseButtonDownEvent& event) {
	if (moveLightState != nullptr)
		moveLightState->processMouseDown(event);
}

void LightEditState::processMouseUp(MouseButtonUpEvent& event) {
	if (moveLightState != nullptr)
		moveLightState->processMouseUp(event);

	if (!event.isHandled) {
		if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT)
			return;

		Camera* camera = context->getCamera();
		std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
		searchBlocksOnRay(intersectedBlocks);
	}
}

void LightEditState::searchBlocksOnRay(const std::vector<Point3di>& intersectedBlocks) {
	for (Point3di blockLocation : intersectedBlocks) {
		Light* lightAtPosition = context->getLightManager()->getLight(blockLocation);
		if (lightAtPosition != nullptr && lightAtPosition != context->getLightManager()->getSelectedLight()) {
			context->getLightManager()->setSelectedLight(lightAtPosition);
			setNewMoveLightState(lightAtPosition);
			break;
		}
	}
}

void LightEditState::setNewMoveLightState(Light* lightToMove) {
	if (moveLightState != nullptr) {
		moveLightState->cleanUp();
		delete moveLightState;
	}
	moveLightState = new MoveLightState(context, lightToMove);
	moveLightState->init();
}

void LightEditState::processMouseMovement(MouseMoveEvent& event) {
	if (moveLightState != nullptr)
		moveLightState->processMouseMovement(event);
}

void LightEditState::onSelectedLightChanged() {
	if (moveLightState != nullptr) {
		moveLightState->cleanUp();
		delete moveLightState;
	}
	moveLightState = new MoveLightState(context, context->getLightManager()->getSelectedLight());
	moveLightState->init();
}

void LightEditState::render() {
	if (moveLightState != nullptr) {
		moveLightState->render();
	} else {
		BaseEditorState::render();
	}
}

void LightEditState::cleanUp() {
	context->getLightManager()->setSelectedLight(nullptr);
}