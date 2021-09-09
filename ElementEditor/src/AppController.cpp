#include "AppController.h"

#include "AddState.h"
#include "SubtractState.h"
#include "SelectState.h"

AppController::AppController(Camera* camera, ModelRenderer* modelRenderer, ChunkManager* modelChunkManager, ChunkManager* previewChunkManager,
	UIRenderer* uiRenderer, Window* window)
	: camera(camera), modelRenderer(modelRenderer), modelChunkManager(modelChunkManager), previewChunkManager(previewChunkManager),
	uiRenderer(uiRenderer), window(window) {
	this->state = new AddState(this);
}

void AppController::addUIController(UIController* uiController) {
	uiController->setController(this);
	uiControllers.push_back(uiController);
}

void AppController::setAddTool() {
	changeActiveTool(new AddState(this));
}

void AppController::setSubtractTool() {
	changeActiveTool(new SubtractState(this));
}

void AppController::setSelectTool() {
	changeActiveTool(new SelectState(this));
}

std::vector<Point3di>* AppController::getSelection() {
	return &selection;
}

void AppController::processMouseMovement(MouseMoveEvent& event) {
	if (window->isClicked(GLFW_MOUSE_BUTTON_MIDDLE)) {
		if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			camera->pan(event.deltaX, event.deltaY);
		} else {
			camera->rotate(event.deltaX, event.deltaY);
		}
	} else {
		state->processMouseMovement(event);
	}
}

void AppController::processScroll(MouseScrollEvent& event) {
	state->processScroll(event);
	if (!event.isHandled) {
		camera->zoom(event.deltaY);
	}
}

void AppController::processKeyPress(KeyPressEvent& event) {
	state->processKeyPress(event);
}

void AppController::processClick(MouseButtonUpEvent& event) {
	state->processClick(event);
}

void AppController::render() {
	state->render();
	for (UIController* uiController : uiControllers) {
		for (UIElement* uiElement : uiController->getUIElements()) {
			uiRenderer->render(*uiElement);
		}
	}
}

Camera* AppController::getCamera() {
	return camera;
}

ChunkManager* AppController::getModelChunkManager() {
	return modelChunkManager;
}

ChunkManager* AppController::getPreviewChunkManager() {
	return previewChunkManager;
}

ModelRenderer* AppController::getModelRenderer() {
	return modelRenderer;
}

Window* AppController::getWindow() {
	return window;
}

void AppController::changeActiveTool(BaseEditorState* newState) {
	if (state != nullptr) {
		state->cleanUp();
		delete state;
	}
	this->state = newState;
}