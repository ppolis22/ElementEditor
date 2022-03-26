#pragma once

#include "UIControl.h"
#include "UIBlock.h"

class Button : public UIControl {
public:
	Button(float x, float y, float width, float height, glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f));
	virtual ~Button();

	virtual void processMouseMovement(MouseMoveEvent& event) override;
	virtual void processMouseDown(MouseButtonDownEvent& event) override;
	virtual void processMouseUp(MouseButtonUpEvent& event) override;

	virtual void renderElement(UIRenderer* renderer) override;

private:
	UIBlock* body;
	glm::vec3 color;
	bool isHovered, isClicked;
};