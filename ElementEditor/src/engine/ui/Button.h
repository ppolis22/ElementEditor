#pragma once

#include "UITexturedElement.h"
#include <string>

class Button : public UIElement {
public:
	Button(float x, float y, float width, float height, const std::string& texturePath, glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f));
	virtual ~Button();

	virtual void processMouseMovement(MouseMoveEvent& event) override;
	virtual void processMouseDown(MouseButtonDownEvent& event) override;
	virtual void processMouseUp(MouseButtonUpEvent& event) override;

	virtual void renderElement(UIRenderer* renderer) override;

private:
	bool isHovered, isClicked;
	glm::vec3 baseColor, hoverColor, clickColor;
	UITexturedElement* icon;
};