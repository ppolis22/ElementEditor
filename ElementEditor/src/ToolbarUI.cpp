#include "ToolbarUI.h"

#include "AppController.h"

ToolbarUI::ToolbarUI(AppController* controller)
	: UIElement(10.0f, 10.0f, 65.0f, 500.0f), controller(controller)
{
	addButton = new Button(20.0f, 20.0f, 45.0f, 45.0f, "textures/add-button-black.png");
	addButton->addListener(this);
	addChild(addButton);

	subtractButton = new Button(20.0f, 75.0f, 45.0f, 45.0f, "textures/subtract-button-black.png");
	subtractButton->addListener(this);
	addChild(subtractButton);

	selectButton = new Button(20.0f, 130.0f, 45.0f, 45.0f, "textures/select-button-black.png");
	selectButton->addListener(this);
	addChild(selectButton);

	moveButton = new Button(20.0f, 185.0f, 45.0f, 45.0f, "textures/add-button-white.png");
	moveButton->addListener(this);
	addChild(moveButton);

	extrudeButton = new Button(20.0f, 240.0f, 45.0f, 45.0f, "textures/select-button-white.png");
	extrudeButton->addListener(this);
	addChild(extrudeButton);

	colorPickerButton = new Button(75.0f, 20.0f, 45.0f, 45.0f, "textures/select-button-white.png");
	colorPickerButton->addListener(this);
	addChild(colorPickerButton);

	colorPreviewBox = new UIElement(20.0f, 315.0f, 100.0f, 25.0f);
	colorPreviewBox->setAlpha(1.0f);
	addChild(colorPreviewBox);

	rSlider = new UISlider(20.0f, 350.0f, 100.0f, 25.0f, 0.0f, 255.0f);
	rSlider->addListener(this);
	rSlider->setMarkerColor(glm::vec3(0.95f, 0.29f, 0.49f));
	addChild(rSlider);

	gSlider = new UISlider(20.0f, 385.0f, 100.0f, 25.0f, 0.0f, 255.0f);
	gSlider->addListener(this);
	gSlider->setMarkerColor(glm::vec3(0.20f, 0.49f, 0.22f));
	addChild(gSlider);

	bSlider = new UISlider(20.0f, 420.0f, 100.0f, 25.0f, 0.0f, 255.0f);
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
	delete colorPickerButton;
	delete colorPreviewBox;
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
	} else if (e.source == colorPickerButton) {
		controller->setColorPickTool();
	} else if (e.source == rSlider || e.source == gSlider || e.source == bSlider) {
		BlockColor newColor{ rSlider->getValue(), gSlider->getValue(), bSlider->getValue() };
		controller->setActiveColor(newColor);
	}
}

void ToolbarUI::update() {
	moveButton->setEnabled(controller->canSetMoveTool());
	extrudeButton->setEnabled(controller->canSetExtrudeTool());

	BlockColor activeColor = controller->getActiveColor();
	rSlider->setValue(activeColor.r);
	gSlider->setValue(activeColor.g);
	bSlider->setValue(activeColor.b);

	colorPreviewBox->setColor(glm::vec3(activeColor.getNormalizedR(), activeColor.getNormalizedG(), activeColor.getNormalizedB()));
}