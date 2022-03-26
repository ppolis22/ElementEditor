#include "UIElement.h"

UIElement::UIElement(float x, float y, float width, float height)
	: x(x), y(y), width(width), height(height) {}

UIElement::~UIElement() {}

float UIElement::getX() {
	return x;
}

void UIElement::setX(float x) {
	this->x = x;
}

float UIElement::getY() {
	return y;
}

void UIElement::setY(float y) {
	this->y = y;
}

float UIElement::getWidth() {
	return width;
}

void UIElement::setWidth(float width) {
	this->width = width;
}

float UIElement::getHeight() {
	return height;
}

void UIElement::setHeight(float height) {
	this->height = height;
}

void UIElement::addChild(UIElement* child) {
	children.push_back(child);
}

bool UIElement::withinBounds(float x, float y) {
	return x >= this->x && x <= (this->x + width) && y >= this->y && y <= (this->y + height);
}

void UIElement::render(UIRenderer* renderer) {
	this->renderElement(renderer);
	for (UIElement* child : children) {
		child->render(renderer);
	}
}

void UIElement::renderElement(UIRenderer* renderer) { /* To be overridden in subclasses */ }

void UIElement::update() {
	// nothing by default, might not need this method
}