#include "BaseEditorState.h"

#include "engine/RayTracer.h"
#include "engine/Window.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "AppController.h"

BaseEditorState::~BaseEditorState() {}

void BaseEditorState::setContext(AppController* context) {
	this->context = context;
}

void BaseEditorState::processMouseMovement(MouseMoveEvent& event) {}

void BaseEditorState::processKeyPress(KeyPressEvent& event) {}

void BaseEditorState::processScroll(MouseScrollEvent& event) {}

void BaseEditorState::processClick(MouseButtonUpEvent& event) {}

void BaseEditorState::render() {
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* chunkManager = context->getChunkManager();
	Camera* camera = context->getCamera();

	for (Chunk& chunk : chunkManager->getAllChunks()) {
		modelRenderer->render(chunk, *camera);
	}
}