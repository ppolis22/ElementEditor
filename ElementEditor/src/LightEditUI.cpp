#include "LightEditUI.h"

#include "AppController.h"

LightEditUI::LightEditUI(AppController* controller)
	: UIElement(10.0f, 10.0f, 65.0f, 500.0f), controller(controller)
{
	addLightButton = new Button(75.0f, 75.0f, 45.0f, 45.0f, "textures/add-light-button-white.png");
	addLightButton->addListener(this);
	addChild(addLightButton);
	buttonMap[addLightButton] = State::ADD_LIGHT;

	editLightButton = new Button(75.0f, 130.0f, 45.0f, 45.0f, "textures/edit-light-button-white.png");
	editLightButton->addListener(this);
	addChild(editLightButton);
	buttonMap[editLightButton] = State::EDIT_LIGHT;

	removeLightButton = new Button(75.0f, 185.0f, 45.0f, 45.0f, "textures/remove-light-button-white.png");
	removeLightButton->addListener(this);
	addChild(removeLightButton);
	buttonMap[removeLightButton] = State::REMOVE_LIGHT;

	strengthSlider = new UISlider(20.0f, 240.0f, 100.0f, 25.0f, 0.0f, 255.0f);
	strengthSlider->addListener(this);
	//strengthSlider->setMarkerColor(glm::vec3(0.95f, 0.29f, 0.49f));
	addChild(strengthSlider);
}

LightEditUI::~LightEditUI() {
	delete addLightButton;
	delete editLightButton;
	delete removeLightButton;
	delete strengthSlider;
}

void LightEditUI::actionPerformed(const ActionEvent& e) {
	if (e.source == strengthSlider) {
		//controller->setLightStrength(strengthSlider->getValue());
	} else if (auto* buttonSource = dynamic_cast<Button*>(e.source)) {
		if (buttonMap.count(buttonSource) != 0) {
			controller->setState(buttonMap[buttonSource]);
		}
	}
}

void LightEditUI::update() {
	addLightButton->setEnabled(controller->canSetAddLightTool());

	//float lightStrength = controller->getLightStrength();
	//strengthSlider->setValue(lightStrength);	// probably have to normalize it into the range?

	State activeTool = controller->getState();
	for (std::map<Button*, State>::iterator it = buttonMap.begin(); it != buttonMap.end(); it++) {
		it->first->setIsActive(it->second == activeTool);
	}
}