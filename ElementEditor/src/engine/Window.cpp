#include "Window.h"
#include "ElementEngineException.h"

float Window::lastCursorXPos = 0;
float Window::lastCursorYPos = 0;
std::vector<InputListener*> Window::listeners;

Window::Window(int width, int height, const char* title) : width(width), height(height) {
	glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfwWindow) {
		throw ElementEngineException("Could not create GLFW window.");
	}

	glfwSetCursorPosCallback(glfwWindow, mouse_callback);
	glfwSetKeyCallback(glfwWindow, key_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);
	glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
	lastCursorXPos = width / 2.0f;
	lastCursorYPos = height / 2.0f;
}

Window::~Window() {}

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

void Window::registerInputListener(InputListener* listener) {
	listeners.push_back(listener);		// support multiple listeners?
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	for (InputListener* listener : listeners) {
		if (listener != nullptr) {
			float deltaX = xpos - lastCursorXPos;
			float deltaY = ypos - lastCursorYPos;
			lastCursorXPos = xpos;
			lastCursorYPos = ypos;
			listener->processMouseMovement(deltaX, deltaY);
		}
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	for (InputListener* listener : listeners) {
		if (listener != nullptr && action == GLFW_PRESS) {
			listener->processKeyPress(key);
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	for (InputListener* listener : listeners) {
		if (listener != nullptr) {
			listener->processScroll(yOffset);
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	for (InputListener* listener : listeners) {
		if (listener != nullptr && action == GLFW_PRESS) {
			listener->processClick(button, lastCursorXPos, lastCursorYPos);
		}
	}
}