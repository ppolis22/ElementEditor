#pragma once

#include "UIController.h"
#include "../Window.h"
#include "Button.h"

class ToolbarController : public UIController {
public:
	ToolbarController(Window* window);
	void update() override;

private:
	Button addButton;
	Button subtractButton;
	Button selectButton;

	void addToolButtonClicked();
	void subtractToolButtonClicked();
	void selectToolButtonClicked();
};