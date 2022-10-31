#include "BlockEditUI.h"

#include "editor/controller/AppController.h"

BlockEditUI::BlockEditUI(AppController* controller, float x, float y)
	: UIElement(x, y, 120.0f, 300.0f), controller(controller)
{
	addButton = new Button(0.0f, 0.0f, 45.0f, 45.0f, "textures/add-button-white.png");
	addButton->addListener(this);
	addChild(addButton);
	buttonMap[addButton] = State::ADD;

	subtractButton = new Button(0.0f, 55.0f, 45.0f, 45.0f, "textures/subtract-button-white.png");
	subtractButton->addListener(this);
	addChild(subtractButton);
	buttonMap[subtractButton] = State::SUBTRACT;

	selectButton = new Button(0.0f, 110.0f, 45.0f, 45.0f, "textures/select-button-white.png");
	selectButton->addListener(this);
	addChild(selectButton);
	buttonMap[selectButton] = State::SELECT;

	colorPickerButton = new Button(55.0f, 0.0f, 45.0f, 45.0f, "textures/picker-button-white.png");
	colorPickerButton->addListener(this);
	addChild(colorPickerButton);
	buttonMap[colorPickerButton] = State::COLOR_PICK;

	moveButton = new Button(55.0f, 55.0f, 45.0f, 45.0f, "textures/move-button-white.png");
	moveButton->addListener(this);
	addChild(moveButton);
	buttonMap[moveButton] = State::MOVE;

	extrudeButton = new Button(55.0f, 110.0f, 45.0f, 45.0f, "textures/extrude-button-white.png");
	extrudeButton->addListener(this);
	addChild(extrudeButton);
	buttonMap[extrudeButton] = State::EXTRUDE;
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