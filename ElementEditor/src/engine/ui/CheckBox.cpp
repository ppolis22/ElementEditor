#include <GLFW/glfw3.h>

#include "CheckBox.h"

const float CheckBox::CHECKBOX_SIZE = 20.0f;
const float CheckBox::CHECKBOX_FRAME_WIDTH = 3.0f;
const float CheckBox::CHECKBOX_FRAME_OFFSET = 12.0f;

CheckBox::CheckBox(float x, float y, const std::string& texturePath, glm::vec3 fillColor)
	: UIElement(x, y, CHECKBOX_SIZE, CHECKBOX_SIZE), 
	value(false), isHovered(false), isClicked(false)
{
	frame = new UITexturedElement(0.0f, CHECKBOX_FRAME_OFFSET, CHECKBOX_SIZE, CHECKBOX_SIZE, "textures/checkbox-frame.png");
	addChild(frame);

	icon = new UITexturedElement(CHECKBOX_SIZE + 10.0f, 0.0f, 45.0f, 45.0f, texturePath, glm::vec3(0.0f, 0.0f, 0.0f));
	addChild(icon);

	float fillSize = CHECKBOX_SIZE - (2 * CHECKBOX_FRAME_WIDTH);
	fill = new UIElement(CHECKBOX_FRAME_WIDTH, CHECKBOX_FRAME_WIDTH + CHECKBOX_FRAME_OFFSET, fillSize, fillSize);
	addChild(fill);

	this->fillColor = fillColor;
	hoverColor = fillColor + 0.15f;
	clickColor = fillColor - 0.15f;
}

CheckBox::~CheckBox() {
	delete fill;
	delete frameLeft;
	delete frameRight;
	delete frameTop;
	delete frameBottom;
}

bool CheckBox::withinBounds(float x, float y) {
	return frame->withinBounds(x, y);
}

void CheckBox::processMouseMovement(MouseMoveEvent& event) {
	isHovered = withinBounds(event.rawX, event.rawY) && enabled;
}

void CheckBox::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && enabled && withinBounds(event.posX, event.posY)) {
		isClicked = true;
		event.isHandled = true;
	}
}

void CheckBox::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && isClicked && withinBounds(event.posX, event.posY)) {
		setValue(!value);
		event.isHandled = true;
	}
	isClicked = false;
}

void CheckBox::renderElement(UIRenderer* renderer) {
	if (isClicked) {
		fill->setColor(clickColor);
	} else if (isHovered) {
		fill->setColor(hoverColor);
	} else {
		fill->setColor(fillColor);
	}
	UIElement::renderElement(renderer);
}

bool CheckBox::getValue() {
	return value;
}

void CheckBox::setValue(bool value) {
	this->value = value;
	fill->setAlpha(value);
	alertListeners();
}

void CheckBox::setEnabled(bool enabled) {
	this->enabled = enabled;
	// TODO update color of frame?
}