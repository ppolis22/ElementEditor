#include "AddState.h"
#include "engine/Window.h"
#include "engine/Camera.h"
#include "ChunkManager.h"
#include "AppController.h"

AddState::AddState(AppController* context)
	: BaseEditorState(context),
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 10.0f) {}

void AddState::processClick(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}

	ChunkManager* modelChunkManager = context->getModelChunkManager();

	if (readyToAdd) {
		modelChunkManager->setBlock(Grass, toAddCoords);
		modelChunkManager->rebuildChunkMeshes();
	}
}

void AddState::processMouseMovement(MouseMoveEvent& event) {
	Camera* camera = context->getCamera();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	ChunkManager* previewChunkManager = context->getPreviewChunkManager();

	std::vector<Point3di> intersectedBlocks = rayTracer.traceRay(camera->getPosition(), camera->getViewMatrix(), event.rawX, event.rawY);
	for (int i = 1; i < intersectedBlocks.size(); i++) {
		if (modelChunkManager->getBlock(intersectedBlocks[i]) != Empty) {
			Point3di targetToAdd = intersectedBlocks[i - 1];
			if (readyToAdd && targetToAdd != toAddCoords) {
				previewChunkManager->setBlock(Empty, toAddCoords);
			}
			if (!readyToAdd || targetToAdd != toAddCoords) {
				toAddCoords = targetToAdd;
				previewChunkManager->setBlock(Grass, toAddCoords);
				previewChunkManager->rebuildChunkMeshes();
			}
			readyToAdd = true;
			return;
		}
	}

	if (readyToAdd) {
		previewChunkManager->setBlock(Empty, toAddCoords);
	}
	readyToAdd = false;
}