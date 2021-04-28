#include "Window.h"
#include "ElementEngineException.h"

Window::Window(int width, int height, const char* title) {
	glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfwWindow) {
		throw ElementEngineException("Could not create GLFW window.");
	}
}

Window::~Window() {}

bool Window::isOpen() {
	return !glfwWindowShouldClose(glfwWindow);
}

void Window::makeCurrentContext() {
	glfwMakeContextCurrent(glfwWindow);
}

void Window::swapBuffers() {
	glfwSwapBuffers(glfwWindow);
}