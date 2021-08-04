#include "Window.h"
#include "ElementEngineException.h"

float Window::lastCursorXPos = 0;
float Window::lastCursorYPos = 0;
MouseListener* Window::listener = nullptr;

Window::Window(int width, int height, const char* title) : width(width), height(height) {
	glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfwWindow) {
		throw ElementEngineException("Could not create GLFW window.");
	}

	glfwSetCursorPosCallback(glfwWindow, mouse_callback);
	lastCursorXPos = width / 2.0f;
	lastCursorYPos = height / 2.0f;
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

bool Window::isKeyPressed(unsigned int keyCode) {
	return glfwGetKey(glfwWindow, keyCode) == GLFW_PRESS;
}

void Window::setMouseCaptureMode(bool enable) {
	if (enable) {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		// turn it off??
	}
}

void Window::registerMouseListener(MouseListener& mouseListener) {
	listener = &mouseListener;		// support multiple listeners?
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (listener != nullptr) {
		float deltaX = xpos - lastCursorXPos;
		float deltaY = ypos - lastCursorYPos;
		lastCursorXPos = xpos;
		lastCursorYPos = ypos;
		listener->processMovement(deltaX, deltaY);
	}
}