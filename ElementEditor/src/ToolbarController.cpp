#include "ToolbarController.h"
#include "../AppController.h"

ToolbarController::ToolbarController(Window* window)
	: addButton(20.0f, 20.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() { this->addToolButtonClicked(); }
	)),
	subtractButton(20.0f, 75.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() { this->subtractToolButtonClicked(); }
	)),
	selectButton(20.0f, 130.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() { this->selectToolButtonClicked(); }
	)),
	moveButton(20.0f, 185.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() { this->moveToolButtonClicked(); }
	)),
	extrudeButton(20.0f, 240.0f, 45.0f, 45.0f, glm::vec3(0.5, 0.5, 0.5), 1.0f, true, new Command(
		[this]() { this->extrudeToolButtonClicked(); }
	))
{
	window->registerEventListener(&addButton);
	window->registerEventListener(&subtractButton);
	window->registerEventListener(&selectButton);
	window->registerEventListener(&moveButton);
	window->registerEventListener(&extrudeButton);

	uiElements.push_back(&addButton);
	uiElements.push_back(&subtractButton);
	uiElements.push_back(&selectButton);
	uiElements.push_back(&moveButton);
	uiElements.push_back(&extrudeButton);
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

void ToolbarController::selectToolButtonClicked() {
	appController->setSelectTool();
}

void ToolbarController::moveToolButtonClicked() {
	appController->setMoveTool();
}

void ToolbarController::extrudeToolButtonClicked() {
	appController->setExtrudeTool();
}