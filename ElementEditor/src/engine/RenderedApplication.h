#pragma once

#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

class Window;

class RenderedApplication {
public:
	RenderedApplication();
	virtual ~RenderedApplication();

	void start(int width = 960, int height = 540, const char* title = "Element Engine Window");

protected:
	virtual void run(Window& window) = 0;		// subclasses must provide their own run behavior
};