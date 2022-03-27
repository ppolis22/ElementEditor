#pragma once

#include "UIElement.h"

class UISlider : public UIElement {
public:
	UISlider(float x, float y, float width, float height, float minValue, float maxValue);
	virtual ~UISlider();

	virtual void processMouseMovement(MouseMoveEvent& event) override;
	virtual void processMouseDown(MouseButtonDownEvent& event) override;
	virtual void processMouseUp(MouseButtonUpEvent& event) override;

	virtual void renderElement(UIRenderer* renderer) override;
	virtual float getValue();
	virtual void setValue(float value);
	virtual void setMarkerColor(glm::vec3 color);

	float calculateValueFromPosition(float position);
	float calculatePositionFromValue(float value);

protected:
	bool isHovered, isClicked;
	UIElement* marker;
	UIElement* line;
	glm::vec3 markerColor;
	glm::vec3 lineColor;
	float value, minValue, maxValue;

	static const float MARKER_WIDTH;
	static const float LINE_HEIGHT;
};