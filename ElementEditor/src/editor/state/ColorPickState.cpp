#include "ColorPickState.h"
#include "editor/controller/AppController.h"

ColorPickState::ColorPickState(AppController* context)
	: BaseEditorState(context) {}

State ColorPickState::getType() {
	return State::COLOR_PICK;
}

void ColorPickState::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	Window* window = context->getWindow();
	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	for (Point3di blockLocation : intersectedBlocks) {
		BlockColor blockColorOnRay = modelChunkManager->getBlockColor(blockLocation);
		if (!blockColorOnRay.isEmpty()) {
			context->setActiveColor(blockColorOnRay);
			break;
		}
	}
}