#pragma once

#include "UIElement.h"

#include "../Texture.h"

class UITexturedElement : public UIElement {

public:
	UITexturedElement(float x, float y, float width, float height, const std::string& texturePath);
	virtual ~UITexturedElement() {};

protected:
	virtual void renderElement(UIRenderer* renderer) override;

private:
	std::string texturePath;
};