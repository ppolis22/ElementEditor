#pragma once

#include "../EventListener.h"
#include "../event/ActionListener.h"
#include "UIRenderer.h"
#include "../Texture.h"
#include <vector>

class UIElement : public EventListener {
public:
	UIElement(float x, float y, float width, float height);
	virtual ~UIElement();

	virtual float getX();
	virtual void setX(float x);
	virtual float getY();
	virtual void setY(float y);
	virtual float getWidth();
	virtual void setWidth(float width);
	virtual float getHeight();
	virtual void setHeight(float height);

	virtual glm::vec3 getColor();
	virtual void setColor(glm::vec3 color);
	virtual float getAlpha();
	virtual void setAlpha(float alpha);

	virtual void processMouseMovement(MouseMoveEvent& event) override {};
	virtual void processKeyPress(KeyPressEvent& event) override {};
	virtual void processScroll(MouseScrollEvent& event) override {};
	virtual void processMouseDown(MouseButtonDownEvent& event) override {};
	virtual void processMouseUp(MouseButtonUpEvent& event) override {};

	virtual bool isEnabled();
	virtual void setEnabled(bool enabled);
	virtual void addListener(ActionListner* listener);

	virtual void addChild(UIElement* child);
	virtual std::vector<UIElement*> getChildren();

	virtual bool withinBounds(float x, float y);
	virtual void render(UIRenderer* renderer);
	virtual void update();

protected:
	float x, y, width, height;
	glm::vec3 color;
	float alpha;
	bool enabled;

	std::vector<UIElement*> children;
	std::vector<ActionListner*> listeners;

	virtual void alertListeners();
	virtual void renderElement(UIRenderer* renderer);
};