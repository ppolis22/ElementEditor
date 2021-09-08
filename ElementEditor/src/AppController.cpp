#include "AppController.h"

#include "AddState.h"
#include "SubtractState.h"

AppController::AppController(BaseEditorState* state, Camera* camera, ModelRenderer* modelRenderer, ChunkManager* chunkManager, 
	UIRenderer* uiRenderer, Window* window)
	: state(state), camera(camera), modelRenderer(modelRenderer), chunkManager(chunkManager), uiRenderer(uiRenderer), window(window) {
	this->state->setContext(this);
}

void AppController::addUIController(UIController* uiController) {
	uiController->setController(this);
	uiControllers.push_back(uiController);
}

void AppController::setAddTool() {
	changeActiveTool(new AddState());
}

void AppController::setSubtractTool() {
	changeActiveTool(new SubtractState());
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

ChunkManager* AppController::getChunkManager() {
	return chunkManager;
}

ModelRenderer* AppController::getModelRenderer() {
	return modelRenderer;
}

Window* AppController::getWindow() {
	return window;
}

void AppController::changeActiveTool(BaseEditorState* newState) {
	if (state != nullptr) {
		delete state;
	}
	this->state = newState;
	this->state->setContext(this);
}