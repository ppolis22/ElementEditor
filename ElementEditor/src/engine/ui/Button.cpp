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

void Button::processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) {
	isHovered = withinBounds(rawX, rawY);
}

void Button::processMouseDown(int buttonCode, float posX, float posY) {
	if (buttonCode == GLFW_MOUSE_BUTTON_LEFT && withinBounds(posX, posY)) {
		isClicked = true;
	}
}

void Button::processMouseUp(int buttonCode, float posX, float posY) {
	if (buttonCode == GLFW_MOUSE_BUTTON_LEFT && isClicked && withinBounds(posX, posY)) {
		onClick->execute();
	}
	isClicked = false;
}

bool Button::withinBounds(float x, float y) {
	return x >= this->x && x <= (this->x + width) && y >= this->y && y <= (this->y + height);
}