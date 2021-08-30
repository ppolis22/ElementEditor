#pragma once

class InputListener {
public:
	virtual void processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) = 0;
	virtual void processKeyPress(int keyCode) = 0;
	virtual void processScroll(float deltaY) = 0;
	virtual void processMouseDown(int buttonCode, float posX, float posY) = 0;
	virtual void processMouseUp(int buttonCode, float posX, float posY) = 0;
};