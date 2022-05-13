#include "AddLightState.h"
#include "../AppController.h"

AddLightState::AddLightState(AppController* context)
	: BaseEditorState(context) {}

State AddLightState::getType() {
	return State::ADD_LIGHT;
}

void AddLightState::init() {
	context->getModelChunkManager()->deselectAll();
}

void AddLightState::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT)
		return;

	if (readyToAdd) {
		context->getLightManager()->setPreviewLightEnabled(false);
		context->addLight(toAddCoords);
	}
}

void AddLightState::processMouseMovement(MouseMoveEvent& event) {
	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	LightManager* lightManager = context->getLightManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.rawX, event.rawY);
	for (int i = 1; i < intersectedBlocks.size(); i++) {
		if (!modelChunkManager->getBlockColor(intersectedBlocks[i]).isEmpty()) {
			Point3di targetToAdd = intersectedBlocks[i - 1];
			if (!readyToAdd || targetToAdd != toAddCoords) {
				toAddCoords = targetToAdd;

				// TODO set preview light from Active Light Color and Active Light Strength
				BlockColor color = context->getActiveColor();
				lightManager->updatePreviewLight(
					glm::vec3(color.getNormalizedR(), color.getNormalizedG(), color.getNormalizedB()),
					toAddCoords,
					10.0f);
			}
			readyToAdd = true;
			return;
		}
	}

	if (readyToAdd) {
		lightManager->setPreviewLightEnabled(false);
	}
	readyToAdd = false;
}

void AddLightState::cleanUp() {
	context->getLightManager()->setPreviewLightEnabled(false);
}