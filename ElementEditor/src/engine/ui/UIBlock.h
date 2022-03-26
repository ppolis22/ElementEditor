#pragma once

#include "UIElement.h"

class UIBlock : public UIElement {
public:
	UIBlock(float x, float y, float width, float height, glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f), float alpha = 1.0f);
	virtual ~UIBlock() {};

	glm::vec3 getColor();
	void setColor(glm::vec3 color);

	float getAlpha();
	void setAlpha(float alpha);

protected:
	virtual void renderElement(UIRenderer* renderer) override;

private:
	glm::vec3 color;
	float alpha;
};