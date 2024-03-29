#pragma once

#include "BlockEditUI.h"
#include "LightEditUI.h"
#include "ProjectEditUI.h"

#include "engine/ui/UIElement.h"
#include "engine/ui/TabbedUIElement.h"
#include "engine/ui/UISlider.h"
#include "engine/event/ActionListener.h"

#include <map>

class AppController;

class ToolbarUI : public UIElement, public ActionListner {
public:
	ToolbarUI(AppController* controller);
	~ToolbarUI();

	void actionPerformed(const ActionEvent& e) override;
	void update() override;

private:
	AppController* controller;

	TabbedUIElement* controlGroups;
	BlockEditUI* blockEditControls;
	LightEditUI* lightEditControls;
	ProjectEditUI* projectEditControls;

	UISlider* rSlider;
	UISlider* gSlider;
	UISlider* bSlider;

	UIElement* colorPreviewBox;
};