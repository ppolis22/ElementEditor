#include "TabbedUIElement.h"

const float TabbedUIElement::BUTTON_WIDTH = 20.0f;
const float TabbedUIElement::BUTTON_HEIGHT = 20.0f;
const float TabbedUIElement::BUTTON_SPACING = 5.0f;
const float TabbedUIElement::BUTTON_OFFSET = 10.0f;

TabbedUIElement::TabbedUIElement(float x, float y, float width, float height)
	: UIElement(x, y, width, height),
	activeTab(-1)
{
	tabRootAnchor = new UIElement(0.0f, 20.0f, 0.0f, 0.0f);
	addChild(tabRootAnchor);

	tabLine = new UIElement(0.0f, BUTTON_HEIGHT, 100.0f, 3.0f);
	tabLine->setColor(glm::vec3(0.75f, 0.75f, 0.75f));
	tabLine->setAlpha(1.0f);
	addChild(tabLine);
}

TabbedUIElement::~TabbedUIElement() {
	delete tabRootAnchor;
	delete tabLine;

	for (Button* button : tabButtons)
		delete button;
}

int TabbedUIElement::addTab(UIElement* rootElement, const std::string& buttonImagePath) {
	float buttonX = (BUTTON_WIDTH + BUTTON_SPACING) * numTabs + BUTTON_OFFSET;
	Button* tabButton = new Button(buttonX, getGlobalY(), BUTTON_WIDTH, BUTTON_HEIGHT, buttonImagePath);
	
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
		tabRootAnchor->removeChild(tabRootElements.at(activeTab));
	
	activeTab = index;
	tabRootAnchor->addChild(tabRootElements.at(activeTab));

	// force update in case state was changed while tab was inactive
	tabRootElements.at(activeTab)->update();

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