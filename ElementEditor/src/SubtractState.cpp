#include "SubtractState.h"
#include "engine/RayTracer.h"
#include "engine/Window.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "AppController.h"

void SubtractState::processClick(MouseButtonUpEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	Window* window = context->getWindow();
	Camera* camera = context->getCamera();
	ChunkManager* chunkManager = context->getChunkManager();

	RayTracer tracer(window->getWidth(), window->getHeight(), camera->getProjectionMatrix(), 10.0f);
	std::vector<Point3di> intersectedBlocks = tracer.traceRay(camera->getPosition(), camera->getViewMatrix(), event.posX, event.posY);
	for (Point3di blockLocation : intersectedBlocks) {
		if (chunkManager->getBlock(blockLocation) != Empty) {
			chunkManager->setBlock(Empty, blockLocation);
			chunkManager->rebuildChunkMeshes();
			break;
		}
	}
}