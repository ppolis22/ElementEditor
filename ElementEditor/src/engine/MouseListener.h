#pragma once

#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

class MouseListener {
public:
	virtual void processMovement(float deltaX, float deltaY) = 0;
};