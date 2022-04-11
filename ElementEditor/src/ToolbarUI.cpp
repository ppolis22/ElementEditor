#include "ToolbarUI.h"

#include "AppController.h"

ToolbarUI::ToolbarUI(AppController* controller)
	: UIElement(10.0f, 10.0f, 65.0f, 500.0f), controller(controller)
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

	rSlider = new UISlider(20.0f, 295.0f, 100.0f, 25.0f, 0.0f, 255.0f);
	rSlider->addListener(this);
	rSlider->setMarkerColor(glm::vec3(0.95f, 0.29f, 0.49f));
	addChild(rSlider);

	gSlider = new UISlider(20.0f, 330.0f, 100.0f, 25.0f, 0.0f, 255.0f);
	gSlider->addListener(this);
	gSlider->setMarkerColor(glm::vec3(0.20f, 0.49f, 0.22f));
	addChild(gSlider);

	bSlider = new UISlider(20.0f, 365.0f, 100.0f, 25.0f, 0.0f, 255.0f);
	bSlider->addListener(this);
	bSlider->setMarkerColor(glm::vec3(0.22f, 0.55f, 0.96f));
	addChild(bSlider);
}

ToolbarUI::~ToolbarUI() {
	delete addButton;
	delete subtractButton;
	delete selectButton;
	delete moveButton;
	delete extrudeButton;
	delete rSlider;
	delete gSlider;
	delete bSlider;
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
	} else if (e.source == rSlider || e.source == gSlider || e.source == bSlider) {
		BlockColor newColor{ rSlider->getValue(), gSlider->getValue(), bSlider->getValue(), false };
		controller->setActiveColor(newColor);
	}
}

void ToolbarUI::update() {
	moveButton->setEnabled(controller->canSetMoveTool());
	extrudeButton->setEnabled(controller->canSetExtrudeTool());
}