#pragma once

#include "../InputListener.h"
#include "../vendor/glm/glm.hpp"

class UIElement : public InputListener {
public:
	UIElement(float x, float y, float width, float height, glm::vec3 color, float alpha, bool isEnabled);
	virtual ~UIElement();

	float getX();
	float getY();
	float getWidth();
	float getHeight();
	virtual glm::vec3 getColor();
	virtual float getAlpha();

	virtual void processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) override;
	virtual void processKeyPress(int keyCode) override;
	virtual void processScroll(float deltaY) override;
	virtual void processMouseDown(int buttonCode, float posX, float posY) override;
	virtual void processMouseUp(int buttonCode, float posX, float posY) override;

protected:
	float x, y, width, height;
	bool isHovered, isClicked, isEnabled;
	glm::vec3 color;
	float alpha;
};