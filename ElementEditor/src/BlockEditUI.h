#pragma once

#include "engine/ui/UIElement.h"
#include "engine/event/ActionListener.h"
#include "engine/ui/Button.h"
#include "engine/ui/UISlider.h"
#include "state/BaseEditorState.h"

#include <map>

class AppController;

class BlockEditUI : public UIElement, public ActionListner {

public:
	BlockEditUI(AppController* controller, float x, float y);
	~BlockEditUI();

	void actionPerformed(const ActionEvent& e) override;
	void update() override;

private:
	AppController* controller;

	Button* addButton;
	Button* subtractButton;
	Button* selectButton;
	Button* moveButton;
	Button* extrudeButton;
	Button* colorPickerButton;

	std::map<Button*, State> buttonMap;
};