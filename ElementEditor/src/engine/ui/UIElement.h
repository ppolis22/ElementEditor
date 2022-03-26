#pragma once

#include "UIRenderer.h"
#include <vector>

class UIElement {
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

	virtual void addChild(UIElement* child);
	virtual bool withinBounds(float x, float y);
	virtual void render(UIRenderer* renderer);
	virtual void update();

protected:
	float x, y, width, height;
	std::vector<UIElement*> children;

	virtual void renderElement(UIRenderer* renderer);
};