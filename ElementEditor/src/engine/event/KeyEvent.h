#pragma once

#include "Event.h"

struct KeyPressEvent : public Event {
	KeyPressEvent(int keyCode)
		:keyCode(keyCode) {}

	int keyCode;
};