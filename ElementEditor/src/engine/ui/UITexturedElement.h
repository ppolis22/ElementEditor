#pragma once

#include "UIElement.h"

#include "../Texture.h"

class UITexturedElement : public UIElement {

public:
	UITexturedElement(float x, float y, float width, float height, const std::string& texturePath, glm::vec3 blendColor = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~UITexturedElement() {};

	void setBlendColor(glm::vec3 blendColor);

protected:
	virtual void renderElement(UIRenderer* renderer) override;

private:
	std::string texturePath;
	glm::vec3 blendColor;
};