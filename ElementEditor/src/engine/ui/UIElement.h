#pragma once

#include "../EventListener.h"
#include "../vendor/glm/glm.hpp"

class UIElement : public EventListener {
public:
	UIElement(float x, float y, float width, float height, glm::vec3 color, float alpha, bool isEnabled);
	virtual ~UIElement();

	float getX();
	float getY();
	float getWidth();
	float getHeight();
	virtual glm::vec3 getColor();
	virtual float getAlpha();

	virtual void processMouseMovement(MouseMoveEvent& event) override;
	virtual void processKeyPress(KeyPressEvent& event) override;
	virtual void processScroll(MouseScrollEvent& event) override;
	virtual void processMouseDown(MouseButtonDownEvent& event) override;
	virtual void processMouseUp(MouseButtonUpEvent& event) override;

protected:
	float x, y, width, height;
	bool isHovered, isClicked, isEnabled;
	glm::vec3 color;
	float alpha;
};