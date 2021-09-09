#include "SelectState.h"
#include "AppController.h"

#include <algorithm>

SelectState::SelectState(AppController* context)
	: BaseEditorState(context),
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 10.0f) {}

void SelectState::processClick(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	Window* window = context->getWindow();
	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.traceRay(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	for (Point3di blockLocation : intersectedBlocks) {
		if (modelChunkManager->getBlock(blockLocation) != Empty) {
			if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				removeFromSelection(blockLocation);
			} else {
				addToSelection(blockLocation);
			}
			break;
		}
	}
}

void SelectState::addToSelection(Point3di& block) {
	std::vector<Point3di>* selection = context->getSelection();
	if (std::find(selection->begin(), selection->end(), block) == selection->end()) {
		context->getSelection()->push_back(block);
		context->getModelChunkManager()->setSelected(true, block);
		context->getModelChunkManager()->rebuildChunkMeshes();
	}
}

void SelectState::removeFromSelection(Point3di& block) {
	std::vector<Point3di>* selection = context->getSelection();
	selection->erase(std::remove(selection->begin(), selection->end(), block), selection->end());
	context->getModelChunkManager()->setSelected(false, block);
	context->getModelChunkManager()->rebuildChunkMeshes();
}