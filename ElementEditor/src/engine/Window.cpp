#include "Window.h"
#include "ElementEngineException.h"

Window::Window(int width, int height, const char* title) : width(width), height(height) {
	glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfwWindow) {
		throw ElementEngineException("Could not create GLFW window.");
	}

	data.lastCursorXPos = width / 2.0f;
	data.lastCursorYPos = height / 2.0f;
	glfwMakeContextCurrent(glfwWindow);
	glfwSetWindowUserPointer(glfwWindow, &data);

	glfwSetCursorPosCallback(glfwWindow, mouse_callback);
	glfwSetKeyCallback(glfwWindow, key_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);
	glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
}

Window::~Window() {}

void Window::update() {
	glfwPollEvents();		// could be glfwWaitEvents() ?
	glfwSwapBuffers(glfwWindow);
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(glfwWindow);
}

void Window::makeCurrentContext() {
	glfwMakeContextCurrent(glfwWindow);
}

bool Window::isKeyPressed(int keyCode) {
	return glfwGetKey(glfwWindow, keyCode) == GLFW_PRESS;
}

bool Window::isClicked(int mouseButtonCode) {
	return glfwGetMouseButton(glfwWindow, mouseButtonCode);
}

void Window::registerInputListener(InputListener* listener) {
	if (listener != nullptr) {
		data.listeners.push_back(listener);
	}
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	float deltaX = xpos - data.lastCursorXPos;
	float deltaY = ypos - data.lastCursorYPos;
	data.lastCursorXPos = xpos;
	data.lastCursorYPos = ypos;
	for (InputListener* listener : data.listeners) {
		listener->processMouseMovement(xpos, ypos, deltaX, deltaY);
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		for (InputListener* listener : data.listeners) {
			listener->processKeyPress(key);
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	for (InputListener* listener : data.listeners) {
		listener->processScroll(yOffset);
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	for (InputListener* listener : data.listeners) {
		if (action == GLFW_PRESS) {
			listener->processMouseDown(button, data.lastCursorXPos, data.lastCursorYPos);
		} else {
			listener->processMouseUp(button, data.lastCursorXPos, data.lastCursorYPos);
		}
	}
}