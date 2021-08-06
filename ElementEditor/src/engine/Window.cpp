#include "Window.h"
#include "ElementEngineException.h"

float Window::lastCursorXPos = 0;
float Window::lastCursorYPos = 0;
InputListener* Window::listener = nullptr;

Window::Window(int width, int height, const char* title) : width(width), height(height) {
	glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfwWindow) {
		throw ElementEngineException("Could not create GLFW window.");
	}

	glfwSetCursorPosCallback(glfwWindow, mouse_callback);
	glfwSetKeyCallback(glfwWindow, key_callback);
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

bool Window::isKeyPressed(int keyCode) {
	return glfwGetKey(glfwWindow, keyCode) == GLFW_PRESS;
}

bool Window::isClicked(int mouseButtonCode) {
	return glfwGetMouseButton(glfwWindow, mouseButtonCode);
}

void Window::setMouseCaptureMode(bool enable) {
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, (enable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
}

void Window::registerInputListener(InputListener* mouseListener) {
	listener = mouseListener;		// support multiple listeners?
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (listener != nullptr) {
		float deltaX = xpos - lastCursorXPos;
		float deltaY = ypos - lastCursorYPos;
		lastCursorXPos = xpos;
		lastCursorYPos = ypos;
		listener->processMouseMovement(deltaX, deltaY);
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (listener != nullptr && action == GLFW_PRESS) {
		listener->processKeyPress(key);
	}
}