#pragma once

#include "UIElement.h"
#include "Button.h"

#include <vector>

class TabbedUIElement : public UIElement, public ActionListner {
public:
	TabbedUIElement(float x, float y, float width, float height);
	~TabbedUIElement();

	void setActiveTab(int index);
	int addTab(UIElement* rootElement, const std::string& buttonImagePath);

	void actionPerformed(const ActionEvent& e) override;

private:
	std::vector<Button*> tabButtons;
	std::vector<UIElement*> tabRootElements;

	// serves as the parent of the active tab root element, position this to
	// set the position of the tab content relative to this element
	UIElement* tabRootAnchor;
	UIElement* tabLine;
	int numTabs;
	int activeTab;

	static const float BUTTON_WIDTH;
	static const float BUTTON_HEIGHT;
	static const float BUTTON_SPACING;
	static const float BUTTON_OFFSET;
};