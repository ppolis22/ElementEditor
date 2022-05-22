#include "ProjectEditUI.h"

#include "AppController.h"

ProjectEditUI::ProjectEditUI(AppController* controller, float x, float y)
	: UIElement(x, y, 120.0f, 300.0f), controller(controller)
{
	saveButton = new Button(0.0f, 0.0f, 45.0f, 45.0f, "textures/save-button-white.png");
	saveButton->addListener(this);
	addChild(saveButton);
}

ProjectEditUI::~ProjectEditUI() {
	delete saveButton;
}

void ProjectEditUI::actionPerformed(const ActionEvent& e) {
	if (e.source == saveButton) {
		controller->saveProject();
	}
}