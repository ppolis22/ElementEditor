#include "ToolbarController.h"
#include "../AppController.h"

ToolbarController::ToolbarController(Window* window)
	: addButton(20.0f, 20.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() { this->addToolButtonClicked(); }
	)),
	subtractButton(20.0f, 75.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() { this->subtractToolButtonClicked(); }
	)) {
	window->registerEventListener(&addButton);
	window->registerEventListener(&subtractButton);

	uiElements.push_back(&addButton);
	uiElements.push_back(&subtractButton);
}

void ToolbarController::update() {
	// TODO update buttons based on app state
}

void ToolbarController::addToolButtonClicked() {
	appController->setAddTool();
}

void ToolbarController::subtractToolButtonClicked() {
	appController->setSubtractTool();
}