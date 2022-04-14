#include <GLFW/glfw3.h>

#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& texturePath, glm::vec3 color)
	: UIElement(x, y, width, height), isHovered(false), isClicked(false)
{
	setColor(color);
	setAlpha(1.0f);
	baseColor = color;
	hoverColor = baseColor + 0.25f;
	clickColor = baseColor - 0.25f;

	icon = new UITexturedElement(x, y, width, height, texturePath);
	addChild(icon);
}

Button::~Button() {
	delete icon;
}

void Button::processMouseMovement(MouseMoveEvent& event) {
	isHovered = withinBounds(event.rawX, event.rawY) && enabled;
}

void Button::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && enabled && withinBounds(event.posX, event.posY)) {
		isClicked = true;
	}
}

void Button::processMouseUp(MouseButtonUpEvent& event) {
	if (event.buttonCode == GLFW_MOUSE_BUTTON_LEFT && isClicked && withinBounds(event.posX, event.posY)) {
		alertListeners();
		event.isHandled = true;
	}
	isClicked = false;
}

void Button::renderElement(UIRenderer* renderer) {
	if (isClicked) {
		setColor(clickColor);
	} else if (isHovered) {
		setColor(hoverColor);
	} else {
		setColor(baseColor);
	}
	UIElement::renderElement(renderer);
}