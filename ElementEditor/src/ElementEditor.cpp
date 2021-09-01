#include "ElementEditor.h"
#include "engine/Window.h"
#include "engine/ModelRenderer.h"
#include "Chunk.h"
#include "engine/RayTracer.h"
#include "engine/Point3d.h"
#include "engine/ui/UIRenderer.h"
#include "engine/ui/Button.h"
#include "engine/ui/Command.h"
#include "SubtractState.h"
#include "AddState.h"

#include "../vendor/glm/glm.hpp"

#include <vector>
#include <algorithm>

int main(void) {
	ElementEditor app;
	app.start();
}

ElementEditor::ElementEditor()
	:state(nullptr) {
	changeEditorState(new SubtractState());
}

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

	Button button1(20.0f, 20.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() {
			this->changeEditorState(new AddState());
		}
	));
	window->registerInputListener(&button1);

	Button button2(20.0f, 75.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() {
			this->changeEditorState(new SubtractState());
		}
	));
	window->registerInputListener(&button2);

	while (window->isOpen()) {
		glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		state->render();

		uiRenderer.render(button1);
		uiRenderer.render(button2);

		window->update();
	}
}

void ElementEditor::processMouseDown(int buttonCode, float posX, float posY) {
	if (buttonCode == GLFW_MOUSE_BUTTON_LEFT) {
		state->processLeftClick(posX, posY);
	}
}

void ElementEditor::processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) {
	if (window->isClicked(GLFW_MOUSE_BUTTON_MIDDLE)) {
		if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			camera.pan(deltaX, deltaY);
		} else {
			camera.rotate(deltaX, deltaY);
		}
	}
}

void ElementEditor::processScroll(float deltaY) {
	camera.zoom(deltaY);
}

void ElementEditor::changeEditorState(EditorState* newState) {
	if (state != nullptr) {
		delete state;
	}
	this->state = newState;
	this->state->setContext(this);
}

Camera* ElementEditor::getCamera() {
	return &camera;
}

ChunkManager* ElementEditor::getChunkManager() {
	return &chunkManager;
}

ModelRenderer* ElementEditor::getModelRenderer() {
	return &modelRenderer;
}