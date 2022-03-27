#include "ToolbarUI.h"

#include "AppController.h"

ToolbarUI::ToolbarUI(AppController* controller)
	: UIElement(10.0f, 10.0f, 65.0f, 295.0f), controller(controller)
{
	addButton = new Button(20.0f, 20.0f, 45.0f, 45.0f);
	addButton->addListener(this);
	addChild(addButton);

	subtractButton = new Button(20.0f, 75.0f, 45.0f, 45.0f);
	subtractButton->addListener(this);
	addChild(subtractButton);

	selectButton = new Button(20.0f, 130.0f, 45.0f, 45.0f);
	selectButton->addListener(this);
	addChild(selectButton);

	moveButton = new Button(20.0f, 185.0f, 45.0f, 45.0f);
	moveButton->addListener(this);
	addChild(moveButton);

	extrudeButton = new Button(20.0f, 240.0f, 45.0f, 45.0f);
	extrudeButton->addListener(this);
	addChild(extrudeButton);

	slider = new UISlider(20.0f, 290.0f, 100.0f, 45.0f);
	slider->addListener(this);
	addChild(slider);
}

ToolbarUI::~ToolbarUI() {
	delete addButton;
	delete subtractButton;
	delete selectButton;
	delete moveButton;
	delete extrudeButton;
	delete slider;
}

void ToolbarUI::actionPerformed(const ActionEvent& e) {
	if (e.source == addButton) {
		controller->setAddTool();
	} else if (e.source == subtractButton) {
		controller->setSubtractTool();
	} else if (e.source == selectButton) {
		controller->setSelectTool();
	} else if (e.source == moveButton) {
		controller->setMoveTool();
	} else if (e.source == extrudeButton) {
		controller->setExtrudeTool();
	}
}