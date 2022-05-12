#pragma once

#include "UIElement.h"
#include "UITexturedElement.h"

class CheckBox : public UIElement {
public:
	CheckBox(float x, float y, const std::string& texturePath, glm::vec3 fillColor = glm::vec3(0.25f, 0.25f, 0.25f));
	virtual ~CheckBox();

	virtual void processMouseMovement(MouseMoveEvent& event) override;
	virtual void processMouseDown(MouseButtonDownEvent& event) override;
	virtual void processMouseUp(MouseButtonUpEvent& event) override;

	virtual bool withinBounds(float x, float y) override;
	virtual void renderElement(UIRenderer* renderer) override;

	bool getValue();
	void setValue(bool isChecked);
	void setEnabled(bool enabled) override;

private:
	bool value, isHovered, isClicked;
	glm::vec3 fillColor, hoverColor, clickColor;
	UIElement* fill;
	UITexturedElement* frame;
	UITexturedElement* icon;

	// TODO replace these individual frame pieces with image with hole in the middle
	UIElement* frameLeft;
	UIElement* frameRight;
	UIElement* frameTop;
	UIElement* frameBottom;

	static const float CHECKBOX_SIZE;
	static const float CHECKBOX_FRAME_WIDTH;
	static const float CHECKBOX_FRAME_OFFSET;
};