#pragma once

#include "engine/ui/UIElement.h"
#include "engine/event/ActionListener.h"
#include "engine/ui/Button.h"
#include "engine/ui/UISlider.h"
#include "state/BaseEditorState.h"

#include <map>

class AppController;

class LightEditUI : public UIElement, public ActionListner {

public:
	LightEditUI(AppController* controller);
	~LightEditUI();

	void actionPerformed(const ActionEvent& e) override;
	void update() override;

private:
	AppController* controller;

	Button* addLightButton;
	Button* editLightButton;
	Button* removeLightButton;

	UISlider* strengthSlider;

	std::map<Button*, State> buttonMap;
};