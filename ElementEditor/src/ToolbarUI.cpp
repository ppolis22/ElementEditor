#include "ToolbarUI.h"

#include "AppController.h"

ToolbarUI::ToolbarUI(AppController* controller)
	: UIElement(10.0f, 10.0f, 65.0f, 500.0f), controller(controller)
{
	blockEditControls = new BlockEditUI(controller);
	lightEditControls = new LightEditUI(controller);

	controlGroups = new TabbedUIElement(10.0f, 10.0f, 120.0f, 500.0f);
	controlGroups->addTab(blockEditControls, "textures/move-button-white.png");
	controlGroups->addTab(lightEditControls, "textures/light-icon-white.png");
	controlGroups->setActiveTab(0);
	addChild(controlGroups);

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
	delete rSlider;
	delete gSlider;
	delete bSlider;
}

void ToolbarUI::actionPerformed(const ActionEvent& e) {
	if (e.source == rSlider || e.source == gSlider || e.source == bSlider) {
		BlockColor newColor{ rSlider->getValue(), gSlider->getValue(), bSlider->getValue() };
		controller->setActiveColor(newColor);
	}
}

void ToolbarUI::update() {
	BlockColor activeColor = controller->getActiveColor();
	rSlider->setValue(activeColor.r);
	gSlider->setValue(activeColor.g);
	bSlider->setValue(activeColor.b);

	colorPreviewBox->setColor(glm::vec3(activeColor.getNormalizedR(), activeColor.getNormalizedG(), activeColor.getNormalizedB()));
}