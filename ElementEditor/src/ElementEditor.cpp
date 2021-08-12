#include "ElementEditor.h"
#include "engine/Window.h"
#include "engine/Renderer.h"
#include "Chunk.h"
#include "engine/MousePicker.h"

int main(void) {
	ElementEditor app;
	app.start();
}

ElementEditor::ElementEditor() {}

ElementEditor::~ElementEditor() {}

void ElementEditor::run() {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	window->registerInputListener(this);

	Chunk chunk(0, 0, 0);
	chunk.setBlock(Stone, 0, 0, 0);
	chunk.setBlock(Stone, 1, 0, 0);
	chunk.setBlock(Stone, 0, 1, 0);
	chunk.setBlock(Stone, 0, 0, 1);
	chunk.rebuildMesh();

	while (window->isOpen()) {
		glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render(chunk, camera);

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
	MousePicker picker(window->getWidth, window->getHeight, camera.getProjectionMatrix);
	picker.getCurrentRay(camera.getViewMatrix, posX, posY);
}