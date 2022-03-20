#include "UIController.h"

void UIController::update() {}

void UIController::setController(AppController* appController) {
	this->appController = appController;
}

std::vector<UIElement*> UIController::getUIElements() {
	return uiElements;
}