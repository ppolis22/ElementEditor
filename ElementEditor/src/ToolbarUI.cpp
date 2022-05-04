#include "ToolbarUI.h"

#include "AppController.h"

ToolbarUI::ToolbarUI(AppController* controller)
	: UIElement(10.0f, 10.0f, 65.0f, 500.0f), controller(controller)
{
	addButton = new Button(20.0f, 20.0f, 45.0f, 45.0f, "textures/add-button-white.png");
	addButton->addListener(this);
	addChild(addButton);
	buttonMap[addButton] = State::ADD;

	subtractButton = new Button(20.0f, 75.0f, 45.0f, 45.0f, "textures/subtract-button-white.png");
	subtractButton->addListener(this);
	addChild(subtractButton);
	buttonMap[subtractButton] = State::SUBTRACT;

	selectButton = new Button(20.0f, 130.0f, 45.0f, 45.0f, "textures/select-button-white.png");
	selectButton->addListener(this);
	addChild(selectButton);
	buttonMap[selectButton] = State::SELECT;

	moveButton = new Button(20.0f, 185.0f, 45.0f, 45.0f, "textures/move-button-white.png");
	moveButton->addListener(this);
	addChild(moveButton);
	buttonMap[moveButton] = State::MOVE;

	extrudeButton = new Button(20.0f, 240.0f, 45.0f, 45.0f, "textures/extrude-button-white.png");
	extrudeButton->addListener(this);
	addChild(extrudeButton);
	buttonMap[extrudeButton] = State::EXTRUDE;

	colorPickerButton = new Button(75.0f, 20.0f, 45.0f, 45.0f, "textures/picker-button-white.png");
	colorPickerButton->addListener(this);
	addChild(colorPickerButton);
	buttonMap[colorPickerButton] = State::COLOR_PICK;

	lightMoveButton = new Button(75.0f, 75.0f, 45.0f, 45.0f, "textures/light-icon-white.png");
	lightMoveButton->addListener(this);
	addChild(lightMoveButton);
	buttonMap[lightMoveButton] = State::MOVE_LIGHT;

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
	if (e.source == rSlider || e.source == gSlider || e.source == bSlider) {
		BlockColor newColor{ rSlider->getValue(), gSlider->getValue(), bSlider->getValue() };
		controller->setActiveColor(newColor);
	} else if (auto* buttonSource = dynamic_cast<Button*>(e.source)) {
		if (buttonMap.count(buttonSource) != 0) {
			controller->setState(buttonMap[buttonSource]);
		}
	}
}

void ToolbarUI::update() {
	moveButton->setEnabled(controller->canSetMoveTool());
	extrudeButton->setEnabled(controller->canSetExtrudeTool());
	lightMoveButton->setEnabled(controller->canSetMoveLightTool());

	BlockColor activeColor = controller->getActiveColor();
	rSlider->setValue(activeColor.r);
	gSlider->setValue(activeColor.g);
	bSlider->setValue(activeColor.b);

	colorPreviewBox->setColor(glm::vec3(activeColor.getNormalizedR(), activeColor.getNormalizedG(), activeColor.getNormalizedB()));

	State activeTool = controller->getState();
	for (std::map<Button*, State>::iterator it = buttonMap.begin(); it != buttonMap.end(); it++) {
		it->first->setIsActive(it->second == activeTool);
	}
}