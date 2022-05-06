#include "RemoveLightState.h"
#include "../AppController.h"

RemoveLightState::RemoveLightState(AppController* context)
	: BaseEditorState(context) {}

State RemoveLightState::getType() {
	return State::REMOVE_LIGHT;
}

void RemoveLightState::init() {
	context->getModelChunkManager()->deselectAll();
}

void RemoveLightState::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT)
		return;

	Camera* camera = context->getCamera();
	LightManager* lightManager = context->getLightManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	for (Point3di blockLocation : intersectedBlocks) {
		Light* lightAtPosition = lightManager->getLight(blockLocation);
		if (lightAtPosition != nullptr) {
			lightManager->removeLight(lightAtPosition);
			break;
		}
	}
}