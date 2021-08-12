#pragma once

#include "Window.h"

class Window;

class RenderedApplication : public InputListener {
public:
	RenderedApplication();
	virtual ~RenderedApplication();

	void start(int width = 960, int height = 540, const char* title = "Element Engine Window");

protected:
	Window* window = nullptr;

	virtual void run() = 0;		// subclasses must provide their own run behavior
	virtual void processMouseMovement(float deltaX, float deltaY) override;
	virtual void processKeyPress(int keyCode) override;
	virtual void processScroll(float deltaY) override;
	virtual void processClick(int buttonCode, float posX, float posY) override;
};