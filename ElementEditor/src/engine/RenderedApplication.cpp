#include "RenderedApplication.h"
#include "Window.h"
#include "ElementEngineException.h"
#include <iostream>

RenderedApplication::RenderedApplication() {}

RenderedApplication::~RenderedApplication() {}

void RenderedApplication::start(int width, int height, const char* title) {
	if (!glfwInit()) {
		throw ElementEngineException("Could not initialize GLFW.");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	try {
		window = &Window(width, height, title);
		glfwSwapInterval(1);
		glewInit();
		std::cout << glGetString(GL_VERSION) << std::endl;

		run();	// runs the overriden custom application code

		// all objects with OpenGL calls in their destructors must be destroyed before now
		glfwTerminate();
	} catch(ElementEngineException& e) {
		glfwTerminate();
		throw e;
	}
}

Window* RenderedApplication::getWindow() {
	return window;
}

void RenderedApplication::processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) {}

void RenderedApplication::processKeyPress(int keyCode) {}

void RenderedApplication::processScroll(float deltaY) {}

void RenderedApplication::processMouseDown(int buttonCode, float posX, float posY) {}

void RenderedApplication::processMouseUp(int buttonCode, float posX, float posY) {}