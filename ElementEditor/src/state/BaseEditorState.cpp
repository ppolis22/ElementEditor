#include "BaseEditorState.h"

#include "../engine/RayTracer.h"
#include "../engine/Window.h"
#include "../engine/Camera.h"
#include "../engine/ModelRenderer.h"
#include "../ChunkManager.h"
#include "../AppController.h"

BaseEditorState::BaseEditorState(AppController* context)
	: context(context) {}

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
	LightManager* lightManager = context->getLightManager();
	std::vector<Light*> lights = lightManager->getLights();

	modelRenderer->renderWithShadows(chunksToRender, lights, lightManager->getDirectionalLightColor(),
		lightManager->getDirectionalLightPosition(), lightManager->getAmbientLightColor(), chunkShader, *camera, 1.0f);

	std::vector<Renderable*> previewChunksToRender;
	for (Chunk* chunk : previewChunkManager->getAllChunks()) {
		previewChunksToRender.push_back(chunk);
	}
	Shader& previewChunkShader = previewChunkManager->getChunkShader();
	modelRenderer->renderNoShadows(previewChunksToRender, lights, lightManager->getDirectionalLightColor(),
		lightManager->getDirectionalLightPosition(), lightManager->getAmbientLightColor(), previewChunkShader, *camera, 0.5f);

	UIRenderer* uiRenderer = context->getUIRenderer();
	for (Light* light : lights) {
		glm::vec3 lightPos = light->getRenderPosition();
		uiRenderer->renderTexturedQuadInScene(lightPos.x, lightPos.y, lightPos.z, 50.0, 50.0, *camera, 
			"textures/add-button-white.png", light->getColor());
	}
}