#pragma once

#include "MouseEvent.h"
#include "KeyEvent.h"

class EventListener {
public:
	virtual ~EventListener() {};

	virtual void processMouseMovement(MouseMoveEvent& event) = 0;
	virtual void processKeyPress(KeyPressEvent& event) = 0;
	virtual void processScroll(MouseScrollEvent& event) = 0;
	virtual void processMouseDown(MouseButtonDownEvent& event) = 0;
	virtual void processMouseUp(MouseButtonUpEvent& event) = 0;
};