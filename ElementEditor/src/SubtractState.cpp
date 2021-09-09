#include "SubtractState.h"
#include "engine/Window.h"
#include "engine/Camera.h"
#include "ChunkManager.h"
#include "AppController.h"

SubtractState::SubtractState(AppController* context)
	: BaseEditorState(context),
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 10.0f) {}

void SubtractState::processClick(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	Window* window = context->getWindow();
	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.traceRay(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	for (Point3di blockLocation : intersectedBlocks) {
		if (modelChunkManager->getBlock(blockLocation) != Empty) {
			modelChunkManager->setBlock(Empty, blockLocation);
			modelChunkManager->rebuildChunkMeshes();
			break;
		}
	}
}