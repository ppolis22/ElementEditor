#include "AddState.h"
#include "engine/RayTracer.h"
#include "engine/Window.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "ElementEditor.h"

void AddState::processLeftClick(float posX, float posY) {
	Window* window = context->getWindow();
	Camera* camera = context->getCamera();
	ChunkManager* chunkManager = context->getChunkManager();

	RayTracer tracer(window->getWidth(), window->getHeight(), camera->getProjectionMatrix(), 10.0f);
	std::vector<Point3di> intersectedBlocks = tracer.traceRay(camera->getPosition(), camera->getViewMatrix(), posX, posY);
	for (int i = 1; i < intersectedBlocks.size(); i++) {
		if (chunkManager->getBlock(intersectedBlocks[i]) != Empty) {
			chunkManager->setBlock(Grass, intersectedBlocks[i - 1]);
			chunkManager->rebuildChunkMeshes();
			break;
		}
	}
}

void AddState::render() {
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* chunkManager = context->getChunkManager();
	Camera* camera = context->getCamera();

	for (Chunk& chunk : chunkManager->getAllChunks()) {
		modelRenderer->render(chunk, *camera);
	}
}