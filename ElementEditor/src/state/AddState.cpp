#include "AddState.h"
#include "../engine/Window.h"
#include "../engine/Camera.h"
#include "../ChunkManager.h"
#include "../AppController.h"

AddState::AddState(AppController* context)
	: BaseEditorState(context) {}

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

	lastBlockWasInBoundsAndEmpty = false;
	std::vector<Point3di> intersectedBlocks = rayTracer.getIntersectingBlocks(camera->getPosition(), camera->getViewMatrix(), event.rawX, event.rawY);
	for (int i = 1; i < intersectedBlocks.size(); i++) {

		if (locationIsEmpty(intersectedBlocks[i])) 
			continue;

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

	if (readyToAdd) {
		previewChunkManager->setBlockColor(BlockColor::EMPTY(), toAddCoords);
		previewChunkManager->rebuildChunkMeshes();
	}
	readyToAdd = false;
}

static bool inBounds(Point3di location, Project* project) {
	return location.x >= 0 && location.y >= 0 && location.z >= 0
		&& location.x < project->getXDim() && location.y < project->getYDim() && location.z < project->getZDim();
}

bool AddState::locationIsEmpty(Point3di location) {
	Project* project = context->getProject();
	if (project->isBounded()) {
		if (context->getModelChunkManager()->getBlockColor(location).isEmpty()) {
			if (inBounds(location, project)) {
				lastBlockWasInBoundsAndEmpty = true;
				return true;
			}
		}
		return !lastBlockWasInBoundsAndEmpty;
	}

	return context->getModelChunkManager()->getBlockColor(location).isEmpty();
}

void AddState::render() {
	renderModelChunks();
	renderPreviewChunks();
	renderLightIcons();
}

void AddState::cleanUp() {
	context->getPreviewChunkManager()->clear();
}