#include "SelectState.h"
#include "editor/controller/AppController.h"

SelectState::SelectState(AppController* context)
	: BaseEditorState(context) {}

State SelectState::getType() {
	return State::SELECT;
}

void SelectState::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT)
		return;

	Camera* camera = context->getCamera();
	Window* window = context->getWindow();

	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	searchBlocksOnRay(intersectedBlocks);
}

void SelectState::searchBlocksOnRay(const std::vector<Point3di>& intersectedBlocks) {
	Window* window = context->getWindow();

	for (Point3di blockLocation : intersectedBlocks) {
		if (!context->getModelChunkManager()->getBlockColor(blockLocation).isEmpty()) {
			if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				removeBlockFromSelection(blockLocation);
			} else {
				addBlockToSelection(blockLocation);
			}
			window->updateUI();		// so move and extrude buttons enable/disable
			break;
		}
	}
}

void SelectState::addBlockToSelection(Point3di& block) {
	context->getModelChunkManager()->setSelected(true, block);
	context->getModelChunkManager()->rebuildChunkMeshes();
}

void SelectState::removeBlockFromSelection(Point3di& block) {
	context->getModelChunkManager()->setSelected(false, block);
	context->getModelChunkManager()->rebuildChunkMeshes();
}