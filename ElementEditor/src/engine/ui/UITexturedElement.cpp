#include "UITexturedElement.h"

UITexturedElement::UITexturedElement(float x, float y, float width, float height, const std::string& texturePath)
	: UIElement(x, y, width, height), texturePath(texturePath) {}

void UITexturedElement::renderElement(UIRenderer* renderer) {
	renderer->renderTexturedQuad(x, y, width, height, texturePath);
}