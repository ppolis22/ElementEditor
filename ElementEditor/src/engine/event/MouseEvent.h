#pragma once

#include "Event.h"

struct MouseMoveEvent : public Event {
	MouseMoveEvent(float rawX, float rawY, float deltaX, float deltaY)
		:rawX(rawX), rawY(rawY), deltaX(deltaX), deltaY(deltaY) {}

	float rawX, rawY, deltaX, deltaY;
};

struct MouseButtonDownEvent : public Event {
	MouseButtonDownEvent(int buttonCode, float posX, float posY)
		:buttonCode(buttonCode), posX(posX), posY(posY) {}

	int buttonCode;
	float posX, posY;
};

struct MouseButtonUpEvent : public Event {
	MouseButtonUpEvent(int buttonCode, float posX, float posY)
		:buttonCode(buttonCode), posX(posX), posY(posY) {}

	int buttonCode;
	float posX, posY;
};

struct MouseScrollEvent : public Event {
	MouseScrollEvent(float deltaY)
		:deltaY(deltaY) {}

	float deltaY;
};