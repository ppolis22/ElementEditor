#include "BaseEditorState.h"

#include "../engine/RayTracer.h"
#include "../engine/Window.h"
#include "../engine/Camera.h"
#include "../engine/ModelRenderer.h"
#include "../ChunkManager.h"
#include "../AppController.h"

BaseEditorState::BaseEditorState(AppController* context)
	: context(context) {}

BaseEditorState::~BaseEditorState() {}

void BaseEditorState::processMouseMovement(MouseMoveEvent& event) {}

void BaseEditorState::processKeyPress(KeyPressEvent& event) {}

void BaseEditorState::processScroll(MouseScrollEvent& event) {}

void BaseEditorState::processMouseDown(MouseButtonDownEvent& event) {}

void BaseEditorState::processMouseUp(MouseButtonUpEvent& event) {}

void BaseEditorState::init() {}

void BaseEditorState::cleanUp() {}

void BaseEditorState::render() {
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	ChunkManager* previewChunkManager = context->getPreviewChunkManager();
	Camera* camera = context->getCamera();

	std::vector<Chunk> allChunks = modelChunkManager->getAllChunks();
	modelRenderer->render(allChunks, *camera);

	std::vector<Chunk> previewChunks = previewChunkManager->getAllChunks();
	modelRenderer->render(previewChunks, *camera);
}