#include "AppController.h"

#include "state/AddState.h"
#include "state/SubtractState.h"
#include "state/SelectState.h"
#include "state/MoveState.h"
#include "state/ExtrudeState.h"
#include "state/ColorPickState.h"
#include "state/LightEditState.h"
#include "state/AddLightState.h"
#include "state/RemoveLightState.h"

AppController::AppController(Camera* camera, ModelRenderer* modelRenderer, ChunkManager* modelChunkManager, 
	ChunkManager* previewChunkManager, LightManager* lightManager, UIRenderer* uiRenderer, Window* window, 
	ProjectBounds* projectBounds, ModelFileWriter* fileWriter, ShadowMapBuilder* shadowMapBuilder)
	: camera(camera), 
	modelRenderer(modelRenderer), 
	modelChunkManager(modelChunkManager), 
	previewChunkManager(previewChunkManager),
	lightManager(lightManager), 
	uiRenderer(uiRenderer), 
	window(window), 
	projectBounds(projectBounds), 
	fileWriter(fileWriter),
	shadowMapBuilder(shadowMapBuilder),
	activeColor(BlockColor{ 0, 0, 0 }) 
{}

void AppController::initialize() {
	changeActiveTool(new AddState(this));
	setActiveColor(BlockColor{ 255, 255, 255 });
	modelChunkManager->rebuildChunkMeshes();
	rebuildShadowMap();
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
	} else if (stateToSet == State::EXTRUDE && canSetExtrudeTool()) {
		changeActiveTool(new ExtrudeState(this, modelChunkManager->getSelected()));
	} else if (stateToSet == State::COLOR_PICK) {
		changeActiveTool(new ColorPickState(this));
	} else if (stateToSet == State::ADD_LIGHT && canSetAddLightTool()) {
		changeActiveTool(new AddLightState(this));
	} else if (stateToSet == State::EDIT_LIGHT) {
		changeActiveTool(new LightEditState(this));
	} else if (stateToSet == State::REMOVE_LIGHT) {
		changeActiveTool(new RemoveLightState(this));
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

bool AppController::canSetAddLightTool() {
	return lightManager->getLightCount() < LightManager::MAX_NUM_LIGHTS;
}

void AppController::setCanEditLights(bool canEdit) {
	this->canEditLights = canEdit;
	State currentState = getState();
	if (currentState == State::EDIT_LIGHT || currentState == State::ADD_LIGHT || currentState == State::REMOVE_LIGHT) {
		changeActiveTool(new SelectState(this));
	}
	window->updateUI();
}

bool AppController::getCanEditLights() {
	return canEditLights;
}

void AppController::saveProject() {
	fileWriter->write();
}

void AppController::rebuildShadowMap() {
	std::vector<Renderable*> chunksToCastShadows;
	for (Chunk* chunk : modelChunkManager->getAllChunks()) {
		chunksToCastShadows.push_back(chunk);
	}
	shadowMapBuilder->buildShadowMap(chunksToCastShadows);
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

ProjectBounds* AppController::getProjectBounds() {
	return projectBounds;
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

void AppController::addLight(Point3di position) {
	lightManager->addLight(glm::vec3(activeColor.getNormalizedR(), activeColor.getNormalizedG(), activeColor.getNormalizedB()), position, 10.0f);
	window->updateUI();

	if (!canSetAddLightTool()) {
		changeActiveTool(new LightEditState(this));
	}
}