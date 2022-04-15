#include "UITexturedElement.h"

UITexturedElement::UITexturedElement(float x, float y, float width, float height, const std::string& texturePath, glm::vec3 blendColor)
	: UIElement(x, y, width, height), texturePath(texturePath), blendColor(blendColor) {}

void UITexturedElement::setBlendColor(glm::vec3 blendColor) {
	this->blendColor = blendColor;
}

void UITexturedElement::renderElement(UIRenderer* renderer) {
	renderer->renderTexturedQuad(x, y, width, height, texturePath, blendColor);
}