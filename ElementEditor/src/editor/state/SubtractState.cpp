#include "SubtractState.h"

#include "editor/controller/ChunkManager.h"
#include "editor/controller/AppController.h"

#include "engine/app/Window.h"
#include "engine/render/Camera.h"

SubtractState::SubtractState(AppController* context)
	: BaseEditorState(context) {}

State SubtractState::getType() {
	return State::SUBTRACT;
}

void SubtractState::init() {
	context->getModelChunkManager()->deselectAll();
}

void SubtractState::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT)
		return;

	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	for (Point3di blockLocation : intersectedBlocks) {
		if (!modelChunkManager->getBlockColor(blockLocation).isEmpty()) {
			modelChunkManager->setSelected(false, blockLocation);
			modelChunkManager->setBlockColor(BlockColor::EMPTY(), blockLocation);
			modelChunkManager->rebuildChunkMeshes();
			context->rebuildShadowMap();
			break;
		}
	}
}