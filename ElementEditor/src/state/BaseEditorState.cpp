#include "BaseEditorState.h"

#include "../engine/RayTracer.h"
#include "../engine/Window.h"
#include "../engine/Camera.h"
#include "../engine/ModelRenderer.h"
#include "../ChunkManager.h"
#include "../AppController.h"

BaseEditorState::BaseEditorState(AppController* context)
	: context(context) ,
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 25.0f) 
{}

void BaseEditorState::renderModelChunks() {
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	LightManager* lightManager = context->getLightManager();
	std::vector<Light*> lights = lightManager->getLights();
	Camera* camera = context->getCamera();
	Shader& chunkShader = modelChunkManager->getChunkShader();

	std::vector<Renderable*> chunksToRender;
	for (Chunk* chunk : modelChunkManager->getAllChunks()) {
		chunksToRender.push_back(chunk);
	}

	modelRenderer->renderWithShadows(chunksToRender, lights, lightManager->getDirectionalLightColor(),
		lightManager->getDirectionalLightPosition(), lightManager->getAmbientLightColor(), chunkShader, *camera, 1.0f);
}

void BaseEditorState::renderPreviewChunks() {
	ChunkManager* previewChunkManager = context->getPreviewChunkManager();
	ModelRenderer* modelRenderer = context->getModelRenderer();
	LightManager* lightManager = context->getLightManager();
	std::vector<Light*> lights = lightManager->getLights();
	Camera* camera = context->getCamera();

	std::vector<Renderable*> previewChunksToRender;
	for (Chunk* chunk : previewChunkManager->getAllChunks()) {
		previewChunksToRender.push_back(chunk);
	}
	Shader& previewChunkShader = previewChunkManager->getChunkShader();
	modelRenderer->renderNoShadows(previewChunksToRender, lights, lightManager->getDirectionalLightColor(),
		lightManager->getDirectionalLightPosition(), lightManager->getAmbientLightColor(), previewChunkShader, *camera, 0.5f);
}

void BaseEditorState::renderLightIcons() {
	std::vector<Light*> lights = context->getLightManager()->getLights();
	UIRenderer* uiRenderer = context->getUIRenderer();
	Camera* camera = context->getCamera();

	for (Light* light : lights) {
		glm::vec3 lightPos = light->getRenderPosition();
		uiRenderer->renderTexturedQuadInScene(lightPos.x, lightPos.y, lightPos.z, 50.0, 50.0, *camera,
			"textures/light-icon-white.png", light->getColor());
	}
}

void BaseEditorState::render() {
	renderModelChunks();
	renderLightIcons();
}