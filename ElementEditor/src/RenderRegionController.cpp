#include "RenderRegionController.h"
#include "../AppController.h"

RenderRegionController::RenderRegionController(Window* window)
	:renderRegion(0, 0, window->getWidth(), window->getHeight(), 
		[this](MouseButtonUpEvent& event) { this->mouseUpInRenderRegion(event); },
		[this](MouseButtonDownEvent& event) { this->mouseDownInRenderRegion(event); },
		[this](MouseScrollEvent& event) { this->scrolled(event); },
		[this](MouseMoveEvent& event) { this->mouseMovedInRenderRegion(event); }, 
		[this](KeyPressEvent& event) { this->keyPressed(event); }) {
	window->registerEventListener(&renderRegion);
}

void RenderRegionController::update() {
	// nothing for now
}

void RenderRegionController::mouseUpInRenderRegion(MouseButtonUpEvent& event) {
	appController->processMouseUp(event);
}

void RenderRegionController::mouseDownInRenderRegion(MouseButtonDownEvent& event) {
	appController->processMouseDown(event);
}

void RenderRegionController::scrolled(MouseScrollEvent& event) {
	appController->processScroll(event);
}

void RenderRegionController::mouseMovedInRenderRegion(MouseMoveEvent& event) {
	appController->processMouseMovement(event);
}

void RenderRegionController::keyPressed(KeyPressEvent& event) {
	appController->processKeyPress(event);
}