#include "SubtractState.h"
#include "../engine/Window.h"
#include "../engine/Camera.h"
#include "../ChunkManager.h"
#include "../AppController.h"

SubtractState::SubtractState(AppController* context)
	: SelectableState(context) {}

void SubtractState::init() {
	context->getModelChunkManager()->deselectAll();
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
		if (!modelChunkManager->getBlockColor(blockLocation).empty) {
			modelChunkManager->setSelected(false, blockLocation);
			modelChunkManager->setBlockColor(BlockColor{ 0, 0, 0, true }, blockLocation);
			modelChunkManager->rebuildChunkMeshes();
			break;
		}
	}
}