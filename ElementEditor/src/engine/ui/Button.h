#pragma once

#include "UIElement.h"

class Button : public UIElement {
public:
	Button(float x, float y, float width, float height, glm::vec3 color, float alpha, bool isEnabled);
	virtual ~Button();

	virtual glm::vec3 getColor();
	virtual void processMouseMovement(float deltaX, float deltaY);
	virtual void processClick(int buttonCode, float posX, float posY);
};