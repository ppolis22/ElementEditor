#pragma once

#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

class InputListener {
public:
	virtual void processMouseMovement(float deltaX, float deltaY) = 0;
	virtual void processKeyPress(int keyCode) = 0;
	virtual void processScroll(float deltaY) = 0;
};