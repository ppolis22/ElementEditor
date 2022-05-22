#pragma once

#include "engine/ui/UIElement.h"
#include "engine/event/ActionListener.h"
#include "engine/ui/Button.h"
#include "engine/ui/UISlider.h"
#include "engine/ui/CheckBox.h"
#include "state/BaseEditorState.h"

#include <map>

class AppController;

class LightEditUI : public UIElement, public ActionListner {

public:
	LightEditUI(AppController* controller, float x, float y);
	~LightEditUI();

	void actionPerformed(const ActionEvent& e) override;
	void update() override;

private:
	AppController* controller;

	Button* addLightButton;
	Button* editLightButton;
	Button* removeLightButton;

	CheckBox* enableEditCheckBox;

	UISlider* strengthSlider;

	UITexturedElement* strengthIcon;

	std::map<Button*, State> buttonMap;
};