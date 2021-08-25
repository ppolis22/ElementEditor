#include "UIElement.h"

UIElement::UIElement(glm::vec2 position, glm::vec2 scale, glm::vec3 color, float alpha)
	: position(position), scale(scale), color(color), alpha(alpha) {}

UIElement::~UIElement() {}

glm::vec2 UIElement::getPosition() {
	return position;
}

glm::vec2 UIElement::getScale() {
	return scale;
}

glm::vec3 UIElement::getColor() {
	return color;
}

float UIElement::getAlpha() {
	return alpha;
}

// To be overridden in subclasses
void UIElement::processMouseMovement(float deltaX, float deltaY) {}
void UIElement::processKeyPress(int keyCode) {}
void UIElement::processScroll(float deltaY) {}
void UIElement::processClick(int buttonCode, float posX, float posY) {}