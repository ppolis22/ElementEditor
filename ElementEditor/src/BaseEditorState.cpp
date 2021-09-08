#include "BaseEditorState.h"

#include "engine/RayTracer.h"
#include "engine/Window.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "AppController.h"

BaseEditorState::BaseEditorState(AppController* context)
	: context(context) {}

BaseEditorState::~BaseEditorState() {}

void BaseEditorState::processMouseMovement(MouseMoveEvent& event) {}

void BaseEditorState::processKeyPress(KeyPressEvent& event) {}

void BaseEditorState::processScroll(MouseScrollEvent& event) {}

void BaseEditorState::processClick(MouseButtonUpEvent& event) {}

void BaseEditorState::render() {
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	ChunkManager* previewChunkManager = context->getPreviewChunkManager();
	Camera* camera = context->getCamera();

	for (Chunk& chunk : modelChunkManager->getAllChunks()) {
		modelRenderer->render(chunk, *camera);
	}

	for (Chunk& chunk : previewChunkManager->getAllChunks()) {
		modelRenderer->renderPreview(chunk, *camera);
	}
}