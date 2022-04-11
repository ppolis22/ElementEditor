#pragma once

#include "engine/ui/UIElement.h"
#include "engine/ui/Button.h"
#include "engine/ui/UISlider.h"
#include "engine/event/ActionListener.h"

class AppController;

class ToolbarUI : public UIElement, public ActionListner {
public:
	ToolbarUI(AppController* controller);
	~ToolbarUI();

	void actionPerformed(const ActionEvent& e) override;
	void update() override;

private:
	AppController* controller;

	Button* addButton;
	Button* subtractButton;
	Button* selectButton;
	Button* moveButton;
	Button* extrudeButton;

	UISlider* rSlider;
	UISlider* gSlider;
	UISlider* bSlider;
};