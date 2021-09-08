#include <GLFW/glfw3.h>

#include "Button.h"

Button::Button(float x, float y, float width, float height, glm::vec3 color, float alpha, bool isEnabled, Command* onClick)
	: UIElement(x, y, width, height, color, alpha, isEnabled) {
	this->onClick = onClick;
}

Button::~Button() {}

glm::vec3 Button::getColor() {
	if (isClicked) {
		return color - 0.25f;
	} else if (isHovered) {
		return color + 0.25f;
	}
	return color;
}

void Button::processMouseMovement(MouseMoveEvent& event) {
	isHovered = withinBounds(event.rawX, event.rawY);
}

void Button::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && withinBounds(event.posX, event.posY)) {
		isClicked = true;
		event.isHandled = true;
	}
}

void Button::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && isClicked && withinBounds(event.posX, event.posY)) {
		onClick->execute();
	}
	isClicked = false;
}

bool Button::withinBounds(float x, float y) {
	return x >= this->x && x <= (this->x + width) && y >= this->y && y <= (this->y + height);
}