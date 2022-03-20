#pragma once

#include "UIController.h"
#include "engine/Window.h"
#include "engine/ui/Button.h"

class ToolbarController : public UIController {
public:
	ToolbarController(Window* window);
	void update() override;

private:
	Button addButton;
	Button subtractButton;
	Button selectButton;
	Button moveButton;
	Button extrudeButton;

	void addToolButtonClicked();
	void subtractToolButtonClicked();
	void selectToolButtonClicked();
	void moveToolButtonClicked();
	void extrudeToolButtonClicked();
};