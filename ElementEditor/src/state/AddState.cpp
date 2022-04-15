#include "AddState.h"
#include "../engine/Window.h"
#include "../engine/Camera.h"
#include "../ChunkManager.h"
#include "../AppController.h"

AddState::AddState(AppController* context)
	: SelectableState(context) {}

State AddState::getType() {
	return State::ADD;
}

void AddState::init() {
	context->getModelChunkManager()->deselectAll();
}

void AddState::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}

	ChunkManager* modelChunkManager = context->getModelChunkManager();

	if (readyToAdd) {
		modelChunkManager->setBlockColor(context->getActiveColor(), toAddCoords);
		modelChunkManager->rebuildChunkMeshes();
	}
}

void AddState::processMouseMovement(MouseMoveEvent& event) {
	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	ChunkManager* previewChunkManager = context->getPreviewChunkManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.rawX, event.rawY);
	for (int i = 1; i < intersectedBlocks.size(); i++) {
		if (!modelChunkManager->getBlockColor(intersectedBlocks[i]).isEmpty()) {
			Point3di targetToAdd = intersectedBlocks[i - 1];
			if (readyToAdd && targetToAdd != toAddCoords) {
				previewChunkManager->setBlockColor(BlockColor::EMPTY(), toAddCoords);
			}
			if (!readyToAdd || targetToAdd != toAddCoords) {
				toAddCoords = targetToAdd;
				previewChunkManager->setBlockColor(context->getActiveColor(), toAddCoords);
				previewChunkManager->rebuildChunkMeshes();
			}
			readyToAdd = true;
			return;
		}
	}

	if (readyToAdd) {
		previewChunkManager->setBlockColor(BlockColor::EMPTY(), toAddCoords);
		previewChunkManager->rebuildChunkMeshes();
	}
	readyToAdd = false;
}

void AddState::cleanUp() {
	context->getPreviewChunkManager()->clear();
}