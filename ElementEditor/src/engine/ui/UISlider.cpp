#include <GLFW/glfw3.h>

#include "UISlider.h"

const float UISlider::MARKER_WIDTH = 10.0f;
const float UISlider::LINE_HEIGHT = 5.0f;

UISlider::UISlider(float x, float y, float width, float height, float minValue, float maxValue)
	: UIElement(x, y, width, height), 
	isHovered(false), isClicked(false), minValue(minValue), maxValue(maxValue), 
	markerColor(glm::vec3(0.4, 0.4, 0.4)), lineColor(glm::vec3(0.75, 0.75, 0.75))
{
	marker = new UIElement(x, y, MARKER_WIDTH, height);
	marker->setColor(markerColor);
	marker->setAlpha(1.0f);

	line = new UIElement(x, y + height / 2.0f, width, LINE_HEIGHT);
	line->setColor(lineColor);
	line->setAlpha(1.0f);

	addChild(line);
	addChild(marker);
}

UISlider::~UISlider() {
	delete marker;
	delete line;
}

float UISlider::getValue() {
	return value;
}

void UISlider::setValue(float value) {
	if (value < minValue || value > maxValue) {
		return;
	}

	this->value = value;
	float markerXPos = calculatePositionFromValue(value);
	marker->setX(markerXPos);
}

void UISlider::setMarkerColor(glm::vec3 color) {
	markerColor = color;
}

void UISlider::processMouseMovement(MouseMoveEvent& event) {
	isHovered = withinBounds(event.rawX, event.rawY) && enabled;
	if (isClicked) {
		float markerXPos = fminf(event.rawX, x + width);
		markerXPos = fmaxf(markerXPos, x);
		marker->setX(markerXPos);

		value = calculateValueFromPosition(markerXPos);
		alertListeners();
	}
}

void UISlider::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && enabled && withinBounds(event.posX, event.posY)) {
		isClicked = true;
	}
}

void UISlider::processMouseUp(MouseButtonUpEvent& event) {
	isClicked = false;
}

void UISlider::renderElement(UIRenderer* renderer) {
	if (isClicked) {
		marker->setColor(markerColor - 0.25f);
	} else if (isHovered) {
		marker->setColor(markerColor + 0.25f);
	} else {
		marker->setColor(markerColor);
	}

	UIElement::renderElement(renderer);
}

float UISlider::calculateValueFromPosition(float position) {
	return (((position - x) / width) * (maxValue - minValue)) + minValue;
}

float UISlider::calculatePositionFromValue(float value) {
	return (((value - minValue) / (maxValue - minValue)) * width) + x;
}