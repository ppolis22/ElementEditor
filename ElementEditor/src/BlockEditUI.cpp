#include "BlockEditUI.h"

#include "AppController.h"

BlockEditUI::BlockEditUI(AppController* controller)
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
}

BlockEditUI::~BlockEditUI() {
	delete addButton;
	delete subtractButton;
	delete selectButton;
	delete moveButton;
	delete extrudeButton;
	delete colorPickerButton;
}

void BlockEditUI::actionPerformed(const ActionEvent& e) {
	if (auto* buttonSource = dynamic_cast<Button*>(e.source)) {
		if (buttonMap.count(buttonSource) != 0) {
			controller->setState(buttonMap[buttonSource]);
		}
	}
}

void BlockEditUI::update() {
	moveButton->setEnabled(controller->canSetMoveTool());
	extrudeButton->setEnabled(controller->canSetExtrudeTool());

	State activeTool = controller->getState();
	for (std::map<Button*, State>::iterator it = buttonMap.begin(); it != buttonMap.end(); it++) {
		it->first->setIsActive(it->second == activeTool);
	}
}