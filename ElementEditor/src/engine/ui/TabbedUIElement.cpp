#include "TabbedUIElement.h"

const float TabbedUIElement::BUTTON_WIDTH = 20.0f;
const float TabbedUIElement::BUTTON_HEIGHT = 20.0f;
const float TabbedUIElement::BUTTON_SPACING = 5.0f;

TabbedUIElement::TabbedUIElement(float x, float y, float width, float height)
	: UIElement(x, y, width, height),
	activeTab(-1)
{}

int TabbedUIElement::addTab(UIElement* rootElement, const std::string& buttonImagePath) {
	float buttonX = x + ((BUTTON_WIDTH + BUTTON_SPACING) * numTabs);
	Button* tabButton = new Button(buttonX, y, BUTTON_WIDTH, BUTTON_HEIGHT, buttonImagePath);
	
	tabButton->addListener(this);
	tabButtons.push_back(tabButton);
	addChild(tabButton);

	tabRootElements.push_back(rootElement);

	setActiveTab(numTabs);
	return numTabs++;
}

void TabbedUIElement::setActiveTab(int index) {
	if (index < 0 || index >= numTabs || index == activeTab)
		return;

	if (activeTab != -1)
		removeChild(tabRootElements.at(activeTab));
	
	activeTab = index;
	addChild(tabRootElements.at(activeTab));

	for (int i = 0; i < numTabs; i++) {
		tabButtons.at(i)->setIsActive(i == activeTab);
		tabRootElements.at(i)->setEnabled(i == activeTab);
	}
}

void TabbedUIElement::actionPerformed(const ActionEvent& e) {
	auto it = std::find(tabButtons.begin(), tabButtons.end(), e.source);
	if (it != tabButtons.end()) {
		int index = it - tabButtons.begin();
		setActiveTab(index);
	}
}