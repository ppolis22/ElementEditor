#pragma once

#include "engine/ui/UIElement.h"
#include "engine/event/ActionListener.h"
#include "engine/ui/Button.h"

class AppController;

class ProjectEditUI : public UIElement, public ActionListner {

public:
	ProjectEditUI(AppController* controller, float x, float y);
	~ProjectEditUI();

	void actionPerformed(const ActionEvent& e) override;

private:
	AppController* controller;
	Button* saveButton;
}; 
