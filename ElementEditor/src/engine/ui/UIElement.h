#pragma once

#include "../InputListener.h"
#include "../vendor/glm/glm.hpp"

class UIElement : public InputListener {
public:
	UIElement(glm::vec2 position, glm::vec2 scale, glm::vec3 color, float alpha);
	virtual ~UIElement();

	glm::vec2 getPosition();
	glm::vec2 getScale();
	glm::vec3 getColor();
	float getAlpha();

	virtual void processMouseMovement(float deltaX, float deltaY);
	virtual void processKeyPress(int keyCode);
	virtual void processScroll(float deltaY);
	virtual void processClick(int buttonCode, float posX, float posY);

private:
	glm::vec2 position;
	glm::vec2 scale;
	glm::vec3 color;
	float alpha;
};