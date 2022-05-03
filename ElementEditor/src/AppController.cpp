#include "AppController.h"

#include "state/AddState.h"
#include "state/SubtractState.h"
#include "state/SelectState.h"
#include "state/MoveState.h"
#include "state/ExtrudeState.h"
#include "state/ColorPickState.h"
#include "state/MoveLightState.h"

AppController::AppController(Camera* camera, ModelRenderer* modelRenderer, ChunkManager* modelChunkManager, ChunkManager* previewChunkManager,
	LightManager* lightManager, UIRenderer* uiRenderer, Window* window)
	: camera(camera), modelRenderer(modelRenderer), modelChunkManager(modelChunkManager), previewChunkManager(previewChunkManager),
	lightManager(lightManager), uiRenderer(uiRenderer), window(window), activeColor(BlockColor{ 0, 0, 0 }) {
	this->state = new AddState(this);
}

void AppController::setState(State stateToSet) {
	if (stateToSet == State::ADD) {
		changeActiveTool(new AddState(this));
	} else if (stateToSet == State::SUBTRACT) {
		changeActiveTool(new SubtractState(this));
	} else if (stateToSet == State::SELECT) {
		changeActiveTool(new SelectState(this));
	} else if (stateToSet == State::MOVE && canSetMoveTool()) {
		changeActiveTool(new MoveState(this, modelChunkManager->getSelected()));
	} else if (stateToSet == State::MOVE_LIGHT && canSetMoveLightTool()) {
		changeActiveTool(new MoveLightState(this, lightManager->getSelectedLight()));
	} else if (stateToSet == State::EXTRUDE && canSetExtrudeTool()) {
		changeActiveTool(new ExtrudeState(this, modelChunkManager->getSelected()));
	} else if (stateToSet == State::COLOR_PICK) {
		changeActiveTool(new ColorPickState(this));
	}
	window->updateUI();
}

State AppController::getState() {
	if (state == nullptr) {
		return State::ERROR;
	}
	return state->getType();
}

bool AppController::canSetMoveTool() {
	return !modelChunkManager->getSelected().empty();
}

bool AppController::canSetExtrudeTool() {
	return !modelChunkManager->getSelected().empty();
}

bool AppController::canSetMoveLightTool() {
	return lightManager->getSelectedLight() != nullptr;
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

void AppController::processMouseUp(MouseButtonUpEvent& event) {
	state->processMouseUp(event);
}

void AppController::processMouseDown(MouseButtonDownEvent& event) {
	state->processMouseDown(event);
}

void AppController::render() {
	state->render();
	UIElement* rootElement = window->getRootUIElement();
	if (rootElement != nullptr) {
		rootElement->render(uiRenderer);
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

LightManager* AppController::getLightManager() {
	return lightManager;
}

ModelRenderer* AppController::getModelRenderer() {
	return modelRenderer;
}

UIRenderer* AppController::getUIRenderer() {
	return uiRenderer;
}

Window* AppController::getWindow() {
	return window;
}

void AppController::changeActiveTool(BaseEditorState* newState) {
	if (state != nullptr) {
		state->cleanUp();
		delete state;
	}
	state = newState;
	state->init();
	window->updateUI();
}

BlockColor AppController::getActiveColor() {
	return activeColor;
}

void AppController::setActiveColor(BlockColor newColor) {
	this->activeColor = newColor;
	window->updateUI();
}