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

	std::vector<Renderable*> chunksToRender;
	for (Chunk* chunk : modelChunkManager->getAllChunks()) {
		chunksToRender.push_back(chunk);
	}
	Shader& chunkShader = modelChunkManager->getChunkShader();
	modelRenderer->render(chunksToRender, chunkShader, *camera);

	std::vector<Renderable*> previewChunksToRender;
	for (Chunk* chunk : previewChunkManager->getAllChunks()) {
		previewChunksToRender.push_back(chunk);
	}
	Shader& previewChunkShader = previewChunkManager->getChunkShader();
	modelRenderer->renderPreview(previewChunksToRender, previewChunkShader, *camera);
}