#include "UIElement.h"

UIElement::UIElement(float x, float y, float width, float height, glm::vec3 color, float alpha, bool isEnabled)
	: x(x), y(y), width(width), height(height), color(color), alpha(alpha), isEnabled(isEnabled),
	isHovered(false), isClicked(false) {}

UIElement::~UIElement() {}

float UIElement::getX() {
	return x;
}

float UIElement::getY() {
	return y;
}

float UIElement::getWidth() {
	return width;
}

float UIElement::getHeight() {
	return height;
}

glm::vec3 UIElement::getColor() {
	return color;
}

float UIElement::getAlpha() {
	return alpha;
}

void UIElement::processMouseMovement(float deltaX, float deltaY) {}
void UIElement::processKeyPress(int keyCode) {}
void UIElement::processScroll(float deltaY) {}
void UIElement::processClick(int buttonCode, float posX, float posY) {}