#pragma once

#include "Window.h"

class Window;

class RenderedApplication {
public:
	RenderedApplication();
	virtual ~RenderedApplication();

	void start(int width = 960, int height = 540, const char* title = "Element Engine Window");

protected:
	Window* window = nullptr;

	virtual void run() = 0;		// subclasses must provide their own run behavior
};