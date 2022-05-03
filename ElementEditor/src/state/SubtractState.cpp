#include "SubtractState.h"
#include "../engine/Window.h"
#include "../engine/Camera.h"
#include "../ChunkManager.h"
#include "../AppController.h"

SubtractState::SubtractState(AppController* context)
	: SelectableState(context) {}

State SubtractState::getType() {
	return State::SUBTRACT;
}

void SubtractState::init() {
	context->getModelChunkManager()->deselectAll();
	context->getLightManager()->setSelectedLight(nullptr);
}

void SubtractState::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	Window* window = context->getWindow();
	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	for (Point3di blockLocation : intersectedBlocks) {
		if (!modelChunkManager->getBlockColor(blockLocation).isEmpty()) {
			modelChunkManager->setSelected(false, blockLocation);
			modelChunkManager->setBlockColor(BlockColor::EMPTY(), blockLocation);
			modelChunkManager->rebuildChunkMeshes();
			break;
		}
	}
}