#include "UIElement.h"
#include <algorithm>

UIElement::UIElement(float x, float y, float width, float height)
	: x(x), y(y), width(width), height(height), color(glm::vec3(0.5f, 0.5f, 0.5f)), alpha(0.0f), enabled(true) {}

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

glm::vec3 UIElement::getColor() {
	return color;
}

void UIElement::setColor(glm::vec3 color) {
	this->color = color;
}

float UIElement::getAlpha() {
	return alpha;
}

void UIElement::setAlpha(float alpha) {
	this->alpha = alpha;
}

void UIElement::addChild(UIElement* child) {
	children.push_back(child);
	child->setParent(this);
}

void UIElement::removeChild(UIElement* child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

std::vector<UIElement*> UIElement::getChildren() {
	return children;
}

void UIElement::setParent(UIElement* parent) {
	this->parent = parent;
}

bool UIElement::isEnabled() {
	return enabled;
}

void UIElement::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void UIElement::addListener(ActionListner* listener) {
	listeners.push_back(listener);
}

void UIElement::alertListeners() {
	for (ActionListner* listener : listeners) {
		listener->actionPerformed({ this });
	}
}

bool UIElement::withinBounds(float x, float y) {
	return x >= this->getGlobalX() && x <= (this->getGlobalX() + width) 
		&& y >= this->getGlobalY() && y <= (this->getGlobalY() + height);
}

float UIElement::getGlobalX() {
	return x + (parent == nullptr ? 0.0f : parent->getGlobalX());
}

float UIElement::getGlobalY() {
	return y + (parent == nullptr ? 0.0f : parent->getGlobalY());
}

void UIElement::render(UIRenderer* renderer) {
	this->renderElement(renderer);
	for (UIElement* child : children) {
		child->render(renderer);
	}
}

void UIElement::renderElement(UIRenderer* renderer) { 
	if (alpha != 0.0f) {
		// TODO create GraphicsContext object with render calls and local origin point, to avoid
		// having to explicitly calculate global x/y
		renderer->renderStaticColoredQuad(getGlobalX(), getGlobalY(), width, height, color, alpha);
	}
}

void UIElement::update() {
	// nothing by default
}