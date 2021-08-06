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
		window->makeCurrentContext();
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

void RenderedApplication::processMouseMovement(float deltaX, float deltaY) {}

void RenderedApplication::processKeyPress(int keyCode) {}