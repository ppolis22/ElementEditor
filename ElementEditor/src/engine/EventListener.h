#pragma once

#include "event/MouseEvent.h"
#include "event/KeyEvent.h"

class EventListener {
public:
	virtual void processMouseMovement(MouseMoveEvent& event) = 0;
	virtual void processKeyPress(KeyPressEvent& event) = 0;
	virtual void processScroll(MouseScrollEvent& event) = 0;
	virtual void processMouseDown(MouseButtonDownEvent& event) = 0;
	virtual void processMouseUp(MouseButtonUpEvent& event) = 0;
};