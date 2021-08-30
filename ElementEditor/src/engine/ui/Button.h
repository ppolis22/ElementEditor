#pragma once

#include "UIElement.h"
#include "Command.h"

class Button : public UIElement {
public:
	Button(float x, float y, float width, float height, glm::vec3 color, float alpha, bool isEnabled, Command* onClick);
	virtual ~Button();

	virtual glm::vec3 getColor();
	virtual void processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) override;
	virtual void processMouseDown(int buttonCode, float posX, float posY) override;
	virtual void processMouseUp(int buttonCode, float posX, float posY) override;

private:
	bool isClickable;
	Command* onClick;

	bool withinBounds(float x, float y);
};