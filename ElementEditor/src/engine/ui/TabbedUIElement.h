#pragma once

#include "UIElement.h"
#include "Button.h"

#include <vector>

class TabbedUIElement : public UIElement, public ActionListner {
public:
	TabbedUIElement(float x, float y, float width, float height);

	void setActiveTab(int index);
	int addTab(UIElement* rootElement, const std::string& buttonImagePath);

	void actionPerformed(const ActionEvent& e) override;

private:
	std::vector<Button*> tabButtons;
	std::vector<UIElement*> tabRootElements;
	int numTabs;
	int activeTab;

	static const float BUTTON_WIDTH;
	static const float BUTTON_HEIGHT;
	static const float BUTTON_SPACING;
};