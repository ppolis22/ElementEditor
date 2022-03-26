#pragma once

#include "UIControl.h"

class UISlider : public UIControl {
public:
	UISlider(float x, float y, float width, float height);
	virtual ~UISlider();

	virtual void processMouseMovement(MouseMoveEvent& event) override;
	virtual void processMouseDown(MouseButtonDownEvent& event) override;
	virtual void processMouseUp(MouseButtonUpEvent& event) override;

	virtual void renderElement(UIRenderer* renderer) override;
	virtual float getValue();
	virtual float setValue(float value);

protected:
	bool isHovered, isClicked;
	UIBlock* marker;
	UIBlock* line;
	glm::vec3 markerColor;
	float value;

	static const float MARKER_WIDTH;
	static const float LINE_HEIGHT;
};