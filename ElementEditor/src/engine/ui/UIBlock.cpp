#include "UIBlock.h"

UIBlock::UIBlock(float x, float y, float width, float height, glm::vec3 color, float alpha) 
	: UIElement(x, y, width, height), color(color), alpha(alpha) {}

glm::vec3 UIBlock::getColor() {
	return color;
}

void UIBlock::setColor(glm::vec3 color) {
	this->color = color;
}

float UIBlock::getAlpha() {
	return alpha;
}

void UIBlock::setAlpha(float alpha) {
	this->alpha = alpha;
}

void UIBlock::renderElement(UIRenderer* renderer) {
	renderer->renderQuad(x, y, width, height, color, alpha);
}