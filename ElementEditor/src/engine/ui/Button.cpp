#include <GLFW/glfw3.h>

#include "Button.h"
#include "UIControl.h"

Button::Button(float x, float y, float width, float height, glm::vec3 color)
	: UIControl(x, y, width, height), isHovered(false), isClicked(false), color(color)
{
	body = new UIBlock(x, y, width, height, color);
	addChild(body);
}

Button::~Button() {}

void Button::processMouseMovement(MouseMoveEvent& event) {
	isHovered = withinBounds(event.rawX, event.rawY);
}

void Button::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && isEnabled && withinBounds(event.posX, event.posY)) {
		isClicked = true;
	}
}

void Button::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && isClicked && withinBounds(event.posX, event.posY)) {
		alertListeners();
		event.isHandled = true;
	}
	isClicked = false;
}

void Button::renderElement(UIRenderer* renderer) {
	if (isClicked) {
		body->setColor(color - 0.25f);
	} else if (isHovered) {
		body->setColor(color + 0.25f);
	}
	body->setColor(color);
}