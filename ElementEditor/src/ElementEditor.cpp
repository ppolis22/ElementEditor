#include "ElementEditor.h"
#include "engine/Window.h"
#include "engine/ModelRenderer.h"
#include "Chunk.h"
#include "engine/RayTracer.h"
#include "engine/Point3d.h"
#include "engine/ui/UIRenderer.h"
#include "engine/ui/Button.h"
#include "../vendor/glm/glm.hpp"

#include <vector>

int main(void) {
	ElementEditor app;
	app.start();
}

ElementEditor::ElementEditor() {}

ElementEditor::~ElementEditor() {}

void ElementEditor::run() {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	window->registerInputListener(this);

	MeshBuilder2d meshBuilder2d;
	UIRenderer uiRenderer(meshBuilder2d, window->getWidth(), window->getHeight());

	chunkManager.setBlock(Stone, { 0, 0, 0 });
	chunkManager.setBlock(Stone, { 1, 0, 0 });
	chunkManager.setBlock(Stone, { 0, 1, 0 });
	chunkManager.setBlock(Stone, { 0, 0, 1 });
	chunkManager.rebuildChunkMeshes();

	Button button(10.0f, 20.0f, 25.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true);
	window->registerInputListener(&button);

	while (window->isOpen()) {
		glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Chunk& chunk : chunkManager.getAllChunks()) {
			renderer.render(chunk, camera);
		}

		uiRenderer.render(button);

		window->swapBuffers();
		glfwPollEvents();		// could be glfwWaitEvents() ?
	}
}

void ElementEditor::processMouseMovement(float deltaX, float deltaY) {
	if (window->isClicked(GLFW_MOUSE_BUTTON_MIDDLE)) {
		if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			camera.pan(deltaX, deltaY);
		} else {
			camera.rotate(deltaX, deltaY);
		}
	}
}

void ElementEditor::processKeyPress(int keyCode) {}

void ElementEditor::processScroll(float deltaY) {
	camera.zoom(deltaY);
}

void ElementEditor::processClick(int buttonCode, float posX, float posY) {
	if (buttonCode == GLFW_MOUSE_BUTTON_LEFT) {
		RayTracer tracer(window->getWidth(), window->getHeight(), camera.getProjectionMatrix(), 10.0f);
		std::vector<Point3di> intersectedBlocks = tracer.traceRay(camera.getPosition(), camera.getViewMatrix(), posX, posY);
		for (Point3di blockLocation : intersectedBlocks) {
			if (chunkManager.getBlock(blockLocation) != Empty) {
				chunkManager.setBlock(Empty, blockLocation);
				chunkManager.rebuildChunkMeshes();
				break;
			}
		}
	}
}