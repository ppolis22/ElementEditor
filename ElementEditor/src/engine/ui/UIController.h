#pragma once

#include <vector>
#include "UIElement.h"

class AppController;

class UIController {
public:
	virtual ~UIController() {};

	virtual void update();
	std::vector<UIElement*> getUIElements();
	void setController(AppController* appController);

protected:
	AppController* appController;
	std::vector<UIElement*> uiElements;
};