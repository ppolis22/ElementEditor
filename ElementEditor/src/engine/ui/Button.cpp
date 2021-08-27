#include "Button.h"

Button::Button(float x, float y, float width, float height, glm::vec3 color, float alpha, bool isEnabled) 
	: UIElement(x, y, width, height, color, alpha, isEnabled) {}

Button::~Button() {}

glm::vec3 Button::getColor() {
	if (isHovered) {
		return color + 0.25f;	// TODO make sure clamping works
	} else if (isClicked) {
		return color - 0.25f;
	}
	return color;
}

void Button::processMouseMovement(float deltaX, float deltaY) {
	// TODO change callback to raw mouse pos
}

void Button::processClick(int buttonCode, float posX, float posY) {
	if (posX >= x && posX <= (x + width) && posY >= y && posY <= (y + height)) {
		isClicked = true;		// TODO add mouse release callback
	}
}