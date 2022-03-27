#include <GLFW/glfw3.h>

#include "UISlider.h"

const float UISlider::MARKER_WIDTH = 10.0f;
const float UISlider::LINE_HEIGHT = 5.0f;

UISlider::UISlider(float x, float y, float width, float height)
	: UIElement(x, y, width, height), 
	isHovered(false), isClicked(false), markerColor(glm::vec3(0.5, 0.5, 0.8)), lineColor(glm::vec3(0.75, 0.75, 0.75))
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
	if (value < 0.0f || value > width) {
		return;
	}

	marker->setX(value + x);
}

void UISlider::processMouseMovement(MouseMoveEvent& event) {
	isHovered = withinBounds(event.rawX, event.rawY) && enabled;
	if (isClicked) {
		float markerXPos = fminf(event.rawX, x + width);
		markerXPos = fmaxf(markerXPos, x);
		marker->setX(markerXPos);
		setValue(markerXPos - x);
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